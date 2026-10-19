#include "DERTerm.hpp"
#include "../BigInt/NaturalNumber.hpp"

namespace MathLib {
    DERTerm::DERTerm(void) : name(), children(), value(), memberTemplate(nullptr), type(Type::Unknown) {}
    DERTerm::DERTerm(Type type, const String& name) : name(name), children(), value(), memberTemplate(nullptr), type(type) {}
    DERTerm::DERTerm(Type type, const String& name, const Array<uint8_t>& value) : name(name), children(), value(value), memberTemplate(nullptr), type(type) {}
    DERTerm::DERTerm(const String& name, const Array<DERTerm>& children) : name(name), children(children), value(), memberTemplate(nullptr), type(Type::Sequence) {}
    DERTerm::DERTerm(const String& name, const DERTerm* memberTemplate) : name(name), children(), value(), memberTemplate(memberTemplate), type(Type::Sequence) {}
    bool DERTerm::Equals(const DERTerm& other) const {
        return type == other.type && value == other.value && name == other.name && (memberTemplate ? (other.memberTemplate && *memberTemplate == *other.memberTemplate) : !other.memberTemplate) && children == other.children;
    }
    String DERTerm::ToString(const Sequence<char>& padding) const {
        const String padd = CollectionToString(padding);
        switch (type) {
            case Type::Unknown: return padd + name + ": null";
            case Type::Sequence: {
                const String padd2 = padd + '\t';
                String ret = padd + name + ": {\n";
                for (const DERTerm& child : children) ret += child.ToString(padd2) + '\n';
                return ret + padd + '}';
            }
            case Type::Integer: return padd + name + ": " + NaturalNumber(value).ToHexString();
            case Type::Date: // TODO: Decode
            case Type::ByteArray: return padd + name + ": {\n" + DumpMemory<uint8_t>(value, 16, false, padd + '\t') + padd + '}';
            case Type::ObjectIdentifier: {
                String ret;
                for (const uint8_t& chr : value) ret += chr;
                for (size_t i = 0; i < SizeOfArray(objectIdentifiers); i += 2) {
                    if (ret != objectIdentifiers[i]) continue;
                    ret = String(objectIdentifiers[i + 1]) + " (" + ret + ')';
                    break;
                }
                return padd + name + ": " + ret;
            }
            case Type::String: {
                String ret = padd + name + ": ";
                for (const uint8_t& chr : value) ret += chr;
                return ret;
            }
            case Type::Bool: return padd + name + ": " + BoolToString(value.At(0));
            default: return "";
        }
    }
    bool DERTerm::CheckTypeCompatibility(Type readType) {
        if (type == Type::Unknown) type = readType;
        return type == readType && (type == Type::Sequence || !memberTemplate);
    }
}