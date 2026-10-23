#include "DER.hpp"
#include "DERTagType.hpp"
#include "DERTagClass.hpp"
#include "../Cryptography/Code/Base64.hpp"
#include "../Interfaces/CountedReadable.hpp"
#include "../Interfaces/Sequence/ByteArray.hpp"
#include "../Interfaces/Sequence/SubSequence.hpp"
#include "../Interfaces/Sequence/ConvertSequence.hpp"

namespace MathLib {
    [[nodiscard]] size_t ParseDERLength(Readable& certificateData) {
        uint8_t data = 0;
        if (!certificateData.Read<uint8_t>(data)) return SIZE_MAX;
        const uint8_t size = data & 0b1111111;
        if (!(data & (1 << 7))) return size;
        if (!size || size == 127) return SIZE_MAX;
        size_t ret = 0;
        for (uint8_t j = 0; j < size; j++) {
            uint8_t tmp = 0;
            if (!certificateData.Read<uint8_t>(tmp)) return SIZE_MAX;
            ret = (ret << 8) | tmp;
        }
        return ret;
    }
    bool ParseDER(DERTerm& certificate, Readable& certificateData) {
        uint8_t encoding = 0;
        if (!certificateData.Read<uint8_t>(encoding)) return false;
        if ((DERTagType)encoding == DERTagType::LongForm) return false;
        const DERTagType tagType = (DERTagType)(encoding & 0b11111);
        const DERTagClass tagClass = (DERTagClass)(encoding >> 6);
        // const bool constructed = encoding & (1 << 5);
        const size_t length = ParseDERLength(certificateData);
        if (length == SIZE_MAX) return false;
        switch (tagClass) {
            case DERTagClass::ContextSpecific: {
                // TODO: Use this information
                CountedReadable counter = certificateData;
                if (!ParseDER(certificate, counter)) return false;
                return length == counter.GetReadBytes();
            }
            case DERTagClass::Universal: switch (tagType) {
                case DERTagType::Null: {
                    if (!certificate.CheckTypeCompatibility(DERTerm::Type::Unknown)) return false;
                    return !length;
                }
                case DERTagType::Integer: {
                    if (!certificate.CheckTypeCompatibility(DERTerm::Type::Integer)) return false;
                    certificate.value = length;
                    return certificateData.ReadCollection<uint8_t>(certificate.value);
                }
                case DERTagType::BitString: {
                    if (!certificate.CheckTypeCompatibility(DERTerm::Type::ByteArray)) return false;
                    uint8_t unusedBits = 0;
                    if (!certificateData.Read<uint8_t>(unusedBits)) return false;
                    if (unusedBits) {
                        // TODO:
                        return false;
                    }
                    certificate.value = length - 1;
                    return certificateData.ReadCollection<uint8_t>(certificate.value);
                }
                case DERTagType::OctetString: {
                    if (!certificate.CheckTypeCompatibility(DERTerm::Type::ByteArray)) return false;
                    certificate.value = length;
                    return certificateData.ReadCollection<uint8_t>(certificate.value);
                }
                case DERTagType::Bool: {
                    if (!certificate.CheckTypeCompatibility(DERTerm::Type::Bool)) return false;
                    if (length != 1) return false;
                    certificate.value = 1;
                    return certificateData.ReadCollection<uint8_t>(certificate.value);
                }
                case DERTagType::ObjectIdentifier: {
                    if (!certificate.CheckTypeCompatibility(DERTerm::Type::ObjectIdentifier)) return false;
                    if (!length) return false;
                    Array<size_t> buffer;
                    size_t curr = 0;
                    for (size_t j = 0; j < length; j++) {
                        uint8_t tmp = 0;
                        if (!certificateData.Read<uint8_t>(tmp)) return false;
                        curr = (curr << 7) | (tmp & ~(1 << 7));
                        if (tmp & (1 << 7)) continue;
                        if (!buffer.Add(curr)) return false;
                        curr = 0;
                    }
                    if (curr) return false;
                    const size_t size = buffer.GetSize();
                    String ret = ToString(buffer.AtUnsafe(0) / 40, 10) + '.' + ToString(buffer.AtUnsafe(0) % 40, 10);
                    for (size_t i = 1; i < size; i++) ret += '.'_M + ToString(buffer.AtUnsafe(i), 10);
                    certificate.value = Array<uint8_t>((const uint8_t*)ret.GetValue(), ret.GetSize());
                    return true;
                }
                case DERTagType::UTF8String:
                case DERTagType::IA5String:
                case DERTagType::T61String:
                case DERTagType::PrintableString: {
                    if (!certificate.CheckTypeCompatibility(DERTerm::Type::String)) return false;
                    certificate.value = length;
                    return certificateData.ReadCollection<uint8_t>(certificate.value);
                }
                case DERTagType::UTCTime:
                case DERTagType::GeneralizedTime: {
                    // TODO: Decode
                    if (!certificate.CheckTypeCompatibility(DERTerm::Type::Date)) return false;
                    certificate.value = length;
                    return certificateData.ReadCollection<uint8_t>(certificate.value);
                }
                case DERTagType::Sequence:
                case DERTagType::Set: {
                    if (!certificate.CheckTypeCompatibility(DERTerm::Type::Sequence)) return false;
                    size_t currentChild = 0;
                    const size_t children = certificate.children.GetSize();
                    size_t index = 0;
                    CountedReadable counter = certificateData;
                    while (true) {
                        if (children <= currentChild) {
                            DERTerm tmp;
                            if (certificate.memberTemplate) tmp = *certificate.memberTemplate;
                            tmp.name = (tmp.name.IsEmpty() ? "Unknown"_M : tmp.name) + ToString(index++, 10);
                            if (!ParseDER(tmp, counter)) return false;
                            if (!certificate.children.Add(tmp)) return false;
                        }
                        else if (!ParseDER(certificate.children.AtUnsafe(currentChild), counter)) return false;
                        currentChild++;
                        const size_t tmp = counter.GetReadBytes();
                        if (length == tmp) break;
                        if (length < tmp) return false;
                    }
                    certificate.memberTemplate = nullptr;
                    return true;
                }
                default: return false;
            }
            default: return false;
        }
    }
    bool ParseDERFromSequence(DERTerm& certificate, const Sequence<uint8_t>& certificateData) {
        ByteArray array = certificateData;
        return ParseDER(certificate, array);
    }
    bool ParseDERFromPEM(DERTerm& certificate, const String& data, const String& label, size_t offset) {
        return ParseDERFromSequence(certificate, DecodePEM(data, label, offset));
    }
    Array<uint8_t> DecodePEM(const String& data, const String& label, size_t offset) {
        const size_t tmpStart = data.Find("-----BEGIN "_M + label + "-----\n", offset);
        if (tmpStart == SIZE_MAX) return Array<uint8_t>();
        const size_t start = tmpStart + 11 + label.GetSize() + 6;
        const size_t end = data.Find("-----END "_M + label + "-----", start);
        if (end == SIZE_MAX) return Array<uint8_t>();
        return Base64().Decrypt(ConvertSequence<uint8_t, char>(SubSequence<char>(data, start, end - 1)), CipherKey());
    }
}