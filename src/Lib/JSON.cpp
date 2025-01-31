#include "JSON.hpp"
#include "String.hpp"

namespace MathLib {
    Expected<JSON> LoadJSON(const String& str, size_t& i) {
        String name = "";
        SkipWhiteSpace(str, i);
        if (str.At(i) == '"') {
            i++;
            while (str.At(i) != '"') name += str.At(i++);
            i++;
            SkipWhiteSpace(str, i);
            if (str.At(i) != ':') return Expected<JSON>();
            i++;
            SkipWhiteSpace(str, i);
        }
        if (str.At(i) == '"') {
            i++;
            String value = "";
            while (str.At(i) != '"') value += str.At(i++);
            i++;
            return Expected<JSON>(JSON(JSON::Type::String, name, value));
        }
        else if (IsDigit(str.At(i))) {
            String value = "";
            while (IsDigit(str.At(i)) || str.At(i) == '.' || str.At(i) == 'e') value += str.At(i++);
            return Expected<JSON>(JSON(JSON::Type::Number, name, value));
        }
        else if (IsAlpha(str.At(i))) {
            String value = "";
            while (IsAlpha(str.At(i))) value += str.At(i++);
            return Expected<JSON>(JSON(JSON::Type::Keyword, name, value));
        }
        else if (str.At(i) == '{' || str.At(i) == '[') {
            const char end = str.At(i) == '{' ? '}' : ']';
            JSON ret = JSON(end == '}' ? JSON::Type::Object : JSON::Type::Array, name, "");
            i++;
            while (true) {
                const Expected<JSON> tmp = LoadJSON(str, i);
                if (!tmp.HasValue()) return Expected<JSON>();
                SkipWhiteSpace(str, i);
                if (!ret.AddChild(tmp.Get())) return Expected<JSON>();
                if (str.At(i) == ',') i++;
                else if (str.At(i) == end) {
                    i++;
                    return ret;
                }
                else return Expected<JSON>();
            }
        }
        else return Expected<JSON>();
    }
    JSON::JSON(void) {}
    JSON::JSON(Type type, const String& name, const String& value) : type(type), name(name), value(value) {}
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
    Expected<JSON> JSON::Find(const String& path) const {
        const Array<String> split = Split(path, "/", false);
        JSON prev = *this;
        for (const String& name : split) {
            bool found = false;
            for (const JSON& child : prev.children) {
                if (child.name == name) {
                    prev = child;
                    found = true;
                    break;
                }
            }
            if (!found) return Expected<JSON>();
        }
        return Expected<JSON>(prev);
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
        return file.Puts(ToString(""));
    }
    bool JSON::Load(Readable& file) {
        size_t i = 0;
        const Expected<JSON> tmp = LoadJSON(file.ReadUntil('\0'), i);
        if (!tmp.HasValue()) return false;
        *this = tmp.Get();
        return true;
    }
    String JSON::ToString(const String& padding) const {
        String ret = padding + (name.IsEmpty() ? "" : '"'_M + name + "\": ");
        if (type == Type::String) return ret + '"'_M + value + '"';
        else if (type == Type::Number || type == Type::Keyword) return ret + value;
        else if (type == Type::Object || type == Type::Array) {
            ret += type == Type::Object ? "{\n" : "[\n";
            for (size_t i = 0; i < children.GetSize(); i++)
                ret += children.At(i).ToString(padding + '\t') + (i + 1 != children.GetSize() ? ",\n" : "\n");
            return ret + padding + (type == Type::Object ? '}' : ']');
        }
        else return ret;
    }
}