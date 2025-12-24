#include "Bencode.hpp"
#include <String.hpp>

bool IsPrintable(char chr) {
    return IsBetween(chr, '!', '~') || MathLib::IsWhiteSpace(chr);
}
Bencode::Bencode(void) : value(), children(), type(Type::End) {}
Bencode::Bencode(const MathLib::Sequence<char>& value, bool number) : value(MathLib::CollectionToString(value)), children(), type(number ? Type::Number : Type::String) {}
Bencode::Bencode(const MathLib::Sequence<Bencode>& children, bool list) : value(), children(MathLib::CollectionToArray<Bencode>(children)), type(list ? Type::List : Type::Dictionary) {}
bool Bencode::Save(MathLib::Writable& file) const {
    switch (type) {
        case Bencode::Type::Number: return file.Write<char>('i') && file.Puts(value) && file.Write<char>('e');
        case Bencode::Type::String: return file.Puts(MathLib::ToString(value.GetSize(), 10)) && file.Write<char>(':') && file.Puts(value);
        case Bencode::Type::List:
        case Bencode::Type::Dictionary: {
            if (!file.Write<char>(type == Bencode::Type::List ? 'l' : 'd')) return false;
            for (const Bencode& child : children)
                if (!child.Save(file)) return false;
            return file.Write<char>('e');
        }
        default: return false;
    }
}
bool Bencode::Load(MathLib::Readable& file) {
    char chr;
    if (!file.Read<char>(chr) || !children.Reset()) return false;
    value = "";
    switch (chr) {
        case 'i': {
            type = Bencode::Type::Number;
            while (true) {
                if (!file.Read<char>(chr)) return false;
                if (chr == 'e') break;
                value += chr;
            }
            return true;
        }
        case 'l':
        case 'd': {
            type = chr == 'l' ? Bencode::Type::List : Bencode::Type::Dictionary;
            while (true) {
                Bencode value;
                if (!value.Load(file)) return false;
                if (value.type == Bencode::Type::End) break;
                if (!children.Add(value)) return false;
            }
            return type == Bencode::Type::List || !(children.GetSize() % 2);
        }
        case '0' ... '9': {
            type = Bencode::Type::String;
            value = chr;
            while (true) {
                if (!file.Read<char>(chr)) return false;
                if (chr == ':') break;
                if (!MathLib::IsDigit(chr)) return false;
                value += chr;
            }
            if (value.GetSize() != 1 && value.At(0) == '0') return false;
            const size_t size = MathLib::StringToNumber(value);
            value = "";
            for (size_t i = 0; i < size; i++) {
                if (!file.Read<char>(chr)) return false;
                value += chr;
            }
            return true;
        }
        case 'e': {
            type = Bencode::Type::End;
            return true;
        }
        default: return false;
    }
}
MathLib::String Bencode::ToString(const MathLib::Sequence<char>& padding) const {
    switch (type) {
        case Bencode::Type::Number: return value;
        case Bencode::Type::String: {
            for (const char& chr : value)
                if (!IsPrintable(chr)) return "<Binary data>";
            return '"'_M + value + '"';
        }
        case Bencode::Type::List: {
            MathLib::String ret = "{\n";
            for (const Bencode& child : children) ret += '\t'_M + padding + child.ToString('\t'_M + padding) + '\n';
            return ret + padding + '}';
        }
        case Bencode::Type::Dictionary: {
            const size_t size = children.GetSize();
            MathLib::String ret = "{\n";
            for (size_t i = 0; i < size; i += 2)
                ret += '\t'_M + padding + children.At(i).ToString() + ": " + children.At(i + 1).ToString('\t'_M + padding) + '\n';
            return ret + padding + '}';
        }
        default: return "";
    }
}
bool Bencode::Equals(const Bencode& other) const {
    return type == other.type && value == other.value && children == other.children;
}