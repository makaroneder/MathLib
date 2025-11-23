#include "JSON.hpp"
#include "String.hpp"

namespace MathLib {
    Expected<JSON> LoadJSON(const Sequence<char>& str, size_t& i) {
        String name = "";
        SkipWhiteSpace(str, i);
        if (str.At(i) == '"') {
            i++;
            while (str.At(i) != '"') name += str.At(i++);
            i++;
            SkipWhiteSpace(str, i);
            if (str.At(i) != ':') return JSON(JSON::Type::String, ""_M, name);
            i++;
            SkipWhiteSpace(str, i);
        }
        if (str.At(i) == '"') {
            i++;
            String value = "";
            while (str.At(i) != '"') value += str.At(i++);
            i++;
            return JSON(JSON::Type::String, name, value);
        }
        if (IsDigit(str.At(i))) {
            String value = "";
            while (IsDigit(str.At(i)) || str.At(i) == '.' || str.At(i) == 'e') value += str.At(i++);
            return JSON(JSON::Type::Number, name, value);
        }
        if (IsAlpha(str.At(i))) {
            String value = "";
            while (IsAlpha(str.At(i))) value += str.At(i++);
            return JSON(JSON::Type::Keyword, name, value);
        }
        if (str.At(i) == '{' || str.At(i) == '[') {
            const char end = str.At(i) == '{' ? '}' : ']';
            JSON ret = JSON(end == '}' ? JSON::Type::Object : JSON::Type::Array, name, ""_M);
            i++;
            SkipWhiteSpace(str, i);
            if (str.At(i) != end) while (true) {
                const Expected<JSON> tmp = LoadJSON(str, i);
                if (!tmp.HasValue()) return Expected<JSON>();
                SkipWhiteSpace(str, i);
                if (!ret.AddChild(tmp.Get())) return Expected<JSON>();
                if (str.At(i) == end) break;
                if (str.At(i) == ',') i++;
                else return Expected<JSON>();
            }
            i++;
            return ret;
        }
        return Expected<JSON>();
    }
    JSON::JSON(void) {}
    JSON::JSON(Type type, const Sequence<char>& name, const Sequence<char>& value) : name(CollectionToString(name)), value(CollectionToString(value)), children(), type(type) {}
    JSON::Type JSON::GetType(void) const {
        return type;
    }
    String JSON::GetValue(void) const {
        return value;
    }
    size_t JSON::GetChildrenCount(void) const {
        return children.GetSize();
    }
    bool JSON::AddChild(const JSON& child) {
        return children.Add(child);
    }
    Expected<JSON> JSON::Find(const Sequence<char>& path) const {
        const Array<String> split = Split(path, "/"_M, false);
        JSON prev = *this;
        for (const Sequence<char>& name : split) {
            bool found = false;
            for (const JSON& child : prev.children) {
                if (child.name != name) continue;
                prev = child;
                found = true;
                break;
            }
            if (!found) return Expected<JSON>();
        }
        return prev;
    }
    Iterator<const JSON> JSON::begin(void) const {
        return children.begin();
    }
    Iterator<const JSON> JSON::end(void) const {
        return children.end();
    }
    Iterator<JSON> JSON::begin(void) {
        return children.begin();
    }
    Iterator<JSON> JSON::end(void) {
        return children.end();
    }
    bool JSON::Save(Writable& file) const {
        return file.Puts(ToString());
    }
    bool JSON::Load(Readable& file) {
        size_t i = 0;
        const Expected<JSON> tmp = LoadJSON(file.ReadUntil('\0'), i);
        if (!tmp.HasValue()) return false;
        *this = tmp.Get();
        return true;
    }
    String JSON::ToString(const Sequence<char>& padding) const {
        String ret = CollectionToString(padding) + (name.IsEmpty() ? "" : '"'_M + name + "\": ");
        if (type == Type::String) return ret + '"'_M + value + '"';
        if (type == Type::Number || type == Type::Keyword) return ret + value;
        if (type == Type::Object || type == Type::Array) {
            ret += type == Type::Object ? "{\n" : "[\n";
            for (size_t i = 0; i < children.GetSize(); i++)
                ret += children.At(i).ToString(CollectionToString(padding) + '\t') + (i + 1 != children.GetSize() ? ",\n" : "\n");
            return ret + padding + (type == Type::Object ? '}' : ']');
        }
        return ret;
    }
    bool JSON::operator==(const JSON& other) const {
        return type == other.type && name == other.name && value == other.value && children == other.children;
    }
    bool JSON::operator!=(const JSON& other) const {
        return !(*this == other);
    }
}