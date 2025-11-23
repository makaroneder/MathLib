#include "AMLObject.hpp"
#include "../../Host.hpp"
#include "../../String.hpp"
#include "AMLFieldFlags.hpp"

namespace MathLib {
    AMLObject::AMLObject(void) {}
    AMLObject::AMLObject(const Sequence<char>& name) : name(CollectionToString(name)), type(Type::Normal) {}
    AMLObject::AMLObject(const Sequence<char>& name, const Sequence<uint64_t>& data) : name(CollectionToString(name)), type(Type::Normal), data(CollectionToArray<uint64_t>(data)) {}
    AMLObject::AMLObject(const Sequence<char>& name, const Sequence<uint64_t>& data, Type type) : name(CollectionToString(name)), type(type), data(CollectionToArray<uint64_t>(data)) {}
    Array<uint64_t> AMLObject::GetData(void) const {
        return data;
    }
    bool AMLObject::AddChild(const AMLObject& child) {
        return children.Add(child);
    }
    String AMLObject::ToString(const Sequence<char>& padding) const {
        if (name == "_HID" || false) {
            String str;
            if (data.GetSize() == 1) {
                const uint16_t vendor = data.At(0) & UINT16_MAX;
                const uint16_t device = data.At(0) >> 16;
                const uint16_t vendorRev = ((vendor & UINT8_MAX) << 8) | vendor >> 8;
                str += ((vendorRev >> 10) & 0x1f) + 'A' - 1;
                str += ((vendorRev >> 5) & 0x1f) + 'A' - 1;
                str += ((vendorRev >> 0) & 0x1f) + 'A' - 1;
                const String tmp = MathLib::ToString(device & UINT8_MAX, 16, 2) + MathLib::ToString(device >> 8, 16, 2);
                for (const char& chr : tmp) str += ToUpper(chr);
            }
            else for (size_t i = 0; i < data.GetSize() - 1; i++) str += (char)data.At(i);
            return CollectionToString(padding) + name + ": " + str;
        }
        String ret = CollectionToString(padding) + name + ": {\n";
        switch (type) {
            case Type::Normal: {
                if (children.IsEmpty()) {
                    if (data.IsEmpty()) return CollectionToString(padding) + name;
                    if (data.GetSize() == 1) return CollectionToString(padding) + name + ": 0x" + MathLib::ToString(data.At(0), 16, 8);
                    for (const uint64_t& x : data) ret += CollectionToString(padding) + "\t0x" + MathLib::ToString(x, 16, 8) + '\n';
                    break;
                }
                for (const AMLObject& child : children) ret += child.ToString(CollectionToString(padding) + '\t') + '\n';
                break;
            }
            case Type::OperationRegion: {
                ret += CollectionToString(padding) + "\tSpace: ";
                switch (data.At(0)) {
                    case 0x01: {
                        ret += "System IO";
                        break;
                    }
                    case 0x02: {
                        ret += "PCI config";
                        break;
                    }
                    case 0x03: {
                        ret += "Embedded control";
                        break;
                    }
                    case 0x04: {
                        ret += "SMBus";
                        break;
                    }
                    case 0x05: {
                        ret += "System CMOS";
                        break;
                    }
                    case 0x06: {
                        ret += "PCI bar target";
                        break;
                    }
                    case 0x07: {
                        ret += "IPMI";
                        break;
                    }
                    case 0x08: {
                        ret += "General purpose IO";
                        break;
                    }
                    case 0x09: {
                        ret += "Generic serial bus";
                        break;
                    }
                    case 0x0a: {
                        ret += "PCC";
                        break;
                    }
                    case 0x80 ... 0xff: {
                        ret += "OEM defined";
                        break;
                    }
                    default: ret += "Unknown";
                }
                ret += " (0x"_M + MathLib::ToString(data.At(0), 16, 2) + ")\n";
                ret += CollectionToString(padding) + "\tOffset: 0x" + MathLib::ToString(data.At(1), 16, 8) + '\n';
                ret += CollectionToString(padding) + "\tLength: 0x" + MathLib::ToString(data.At(2), 16, 8) + '\n';
                break;
            }
            case Type::Field: {
                const AMLFieldFlags flags = AMLFieldFlags(data.At(0));
                ret += CollectionToString(padding) + "\tLock: " + BoolToString(flags.lock) + '\n';
                ret += CollectionToString(padding) + "\tAccess type: ";
                switch ((AMLFieldFlags::AccessType)flags.accessType) {
                    case AMLFieldFlags::AccessType::Any: {
                        ret += "Any";
                        break;
                    }
                    case AMLFieldFlags::AccessType::Byte: {
                        ret += "Byte";
                        break;
                    }
                    case AMLFieldFlags::AccessType::Word: {
                        ret += "Word";
                        break;
                    }
                    case AMLFieldFlags::AccessType::DWord: {
                        ret += "DWord";
                        break;
                    }
                    case AMLFieldFlags::AccessType::QWord: {
                        ret += "QWord";
                        break;
                    }
                    case AMLFieldFlags::AccessType::Buffer: {
                        ret += "Buffer";
                        break;
                    }
                    default: ret += "Unknown";
                }
                ret += '\n'_M + padding + "\tAccess type: ";
                switch ((AMLFieldFlags::UpdateRule)flags.updateRule) {
                    case AMLFieldFlags::UpdateRule::Preserve: {
                        ret += "Preserve";
                        break;
                    }
                    case AMLFieldFlags::UpdateRule::FillWith1: {
                        ret += "Fill with 1";
                        break;
                    }
                    case AMLFieldFlags::UpdateRule::FillWith0: {
                        ret += "Fill with 0";
                        break;
                    }
                    default: ret += "Unknown";
                }
                ret += '\n';
                for (const AMLObject& child : children) ret += child.ToString(CollectionToString(padding) + '\t') + '\n';
                break;
            }
        }
        return ret + padding + '}';
    }
    bool AMLObject::operator==(const AMLObject& other) const {
        return type == other.type && name == other.name && data == other.data && children == other.children;
    }
    bool AMLObject::operator!=(const AMLObject& other) const {
        return !(*this == other);
    }
}