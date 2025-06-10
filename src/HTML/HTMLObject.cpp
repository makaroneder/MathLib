#include "HTMLObject.hpp"
#include <FunctionT.hpp>
#include <String.hpp>
#include <Utils.hpp>

HTMLObject::HTMLObject(void) : value(), attributes(), children(), type() {}
HTMLObject::HTMLObject(const MathLib::Sequence<char>& value, bool comment) : value(MathLib::CollectionToString(value)), attributes(), children(), type(comment ? Type::Comment : Type::Data) {}
HTMLObject::HTMLObject(const MathLib::Sequence<char>& name, const MathLib::Sequence<HTMLAttribute>& attributes, const MathLib::Sequence<HTMLObject>& children) : value(MathLib::CollectionToString(name)), attributes(MathLib::CollectionToArray<HTMLAttribute>(attributes)), children(MathLib::CollectionToArray<HTMLObject>(children)), type(Type::Element) {}
MathLib::String HTMLObject::ToString(const MathLib::Sequence<char>& padding) const {
    const MathLib::String padd = MathLib::CollectionToString(padding);
    switch (type) {
        case Type::Data: return padd + value;
        case Type::Comment: return padd + "<!--\n\t" + padd + value + '\n' + padd + "-->";
        case Type::Element : {
            const MathLib::String padd2 = padd + '\t';
            MathLib::String ret = padd + '<' + value;
            for (const HTMLAttribute& attribute : attributes) ret += ' '_M + attribute.ToString();
            ret += ">\n";
            for (const HTMLObject& child : children) ret += child.ToString(padd2) + '\n';
            return ret + padd + "</" + value + '>';
        }
        default: return padd + "<!--Unknown element-->";
    }
}
bool HTMLObject::Equals(const MathLib::Comparable& other_) const {
    const HTMLObject& other = (const HTMLObject&)other_;
    return type == other.type && value == other.value && attributes == other.attributes && children == other.children;
}
bool ParseEnd(const MathLib::Sequence<char>& str, size_t& i, bool& empty) {
    if (str.At(i) == '>') {
        i++;
        return true;
    }
    if (str.At(i) == '/') {
        const size_t save = i;
        i++;
        MathLib::SkipWhiteSpace(str, i);
        if (str.At(i) == '>') {
            empty = true;
            i++;
            return true;
        }
        i = save;
    }
    return false;
}
MathLib::String ReadText(const MathLib::Sequence<char>& str, size_t& i, const MathLib::Function<bool>& func) {
    MathLib::SkipWhiteSpace(str, i);
    MathLib::String ret;
    size_t del = 0;
    while (!func()) {
        const char tmp = str.At(i++);
        if (del) del = (del + 1) * MathLib::IsWhiteSpace(tmp);
        else del = tmp == '\n';
        ret += tmp;
    }
    return MathLib::SubString(ret, 0, ret.GetSize() - del);
}
HTMLObject LoadHTMLObject(const MathLib::Sequence<char>& str, size_t& i) {
    MathLib::SkipWhiteSpace(str, i);
    if (str.At(i) == '<') {
        i++;
        MathLib::SkipWhiteSpace(str, i);
        MathLib::String name;
        bool empty = false;
        bool end = false;
        while (!MathLib::IsWhiteSpace(str.At(i))) {
            if (ParseEnd(str, i, empty)) {
                end = true;
                break;
            }
            name += str.At(i++);
        }
        if (name == "!--") return HTMLObject(ReadText(str, i, MathLib::MakeFunctionT<bool>(nullptr, [&str, &i](const void*) -> bool {
            if (str.At(i) == '-' && str.At(i + 1) == '-' && str.At(i + 2) == '>') {
                i += 3;
                return true;
            }
            return false;
        })), true);
        MathLib::Array<HTMLAttribute> attributes;
        if (!end) while (true) {
            MathLib::SkipWhiteSpace(str, i);
            if (ParseEnd(str, i, empty)) break;
            MathLib::String name;
            while (str.At(i) != '=' && !MathLib::IsWhiteSpace(str.At(i))) name += str.At(i++);
            MathLib::SkipWhiteSpace(str, i);
            if (str.At(i) != '=') return HTMLObject();
            i++;
            MathLib::SkipWhiteSpace(str, i);
            if (str.At(i) != '"') return HTMLObject();
            i++;
            MathLib::String value;
            while (str.At(i) != '"') value += str.At(i++);
            i++;
            if (!attributes.Add(HTMLAttribute(name, value))) return HTMLObject();
        }
        MathLib::Array<HTMLObject> children;
        const bool voidElement = name == "area" || name == "base" || name == "br" || name == "col" || name == "embed" || name == "hr" || name == "img" || name == "input" || name == "keygen" || name == "link" || name == "menuitem" || name == "meta" || name == "param" || name == "source" || name == "track" || name == "wbr";
        if (!empty) {
            while (true) {
                MathLib::SkipWhiteSpace(str, i);
                if (voidElement || (str.At(i) == '<' && str.At(i + 1) == '/')) break;
                if (!children.Add(LoadHTMLObject(str, i))) return HTMLObject();
            }
            if (str.At(i) == '<' && str.At(i + 1) == '/') {
                while (str.At(i) != '>') i++;
                i++;
            }
        }
        return HTMLObject(name, attributes, children);
    }
    return HTMLObject(ReadText(str, i, MathLib::MakeFunctionT<bool>(nullptr, [&str, &i](const void*) -> bool {
        return i >= str.GetSize() || str.At(i) == '<';
    })), false);
}