#include "JSON.hpp"
#include "String.hpp"

namespace MathLib {
    Expected<JSON> LoadJSON(const Sequence<char>& str, size_t& i) {
        StartBenchmark
        String name = "";
        SkipWhiteSpace(str, i);
        if (str.At(i) == '"') {
            i++;
            while (str.At(i) != '"') name += str.At(i++);
            i++;
            SkipWhiteSpace(str, i);
            if (str.At(i) != ':') ReturnFromBenchmark(Expected<JSON>());
            i++;
            SkipWhiteSpace(str, i);
        }
        if (str.At(i) == '"') {
            i++;
            String value = "";
            while (str.At(i) != '"') value += str.At(i++);
            i++;
            ReturnFromBenchmark(JSON(JSON::Type::String, name, value));
        }
        else if (IsDigit(str.At(i))) {
            String value = "";
            while (IsDigit(str.At(i)) || str.At(i) == '.' || str.At(i) == 'e') value += str.At(i++);
            ReturnFromBenchmark(JSON(JSON::Type::Number, name, value));
        }
        else if (IsAlpha(str.At(i))) {
            String value = "";
            while (IsAlpha(str.At(i))) value += str.At(i++);
            ReturnFromBenchmark(JSON(JSON::Type::Keyword, name, value));
        }
        else if (str.At(i) == '{' || str.At(i) == '[') {
            const char end = str.At(i) == '{' ? '}' : ']';
            JSON ret = JSON(end == '}' ? JSON::Type::Object : JSON::Type::Array, name, ""_M);
            i++;
            while (true) {
                const Expected<JSON> tmp = LoadJSON(str, i);
                if (!tmp.HasValue()) ReturnFromBenchmark(Expected<JSON>());
                SkipWhiteSpace(str, i);
                if (!ret.AddChild(tmp.Get())) ReturnFromBenchmark(Expected<JSON>());
                if (str.At(i) == ',') i++;
                else if (str.At(i) == end) {
                    i++;
                    return ret;
                }
                else ReturnFromBenchmark(Expected<JSON>());
            }
        }
        else ReturnFromBenchmark(Expected<JSON>());
    }
    JSON::JSON(void) {
        EmptyBenchmark
    }
    JSON::JSON(Type type, const Sequence<char>& name, const Sequence<char>& value) : name(CollectionToString(name)), value(CollectionToString(value)), children(), type(type) {
        EmptyBenchmark
    }
    JSON::Type JSON::GetType(void) const {
        StartAndReturnFromBenchmark(type);
    }
    String JSON::GetValue(void) const {
        StartAndReturnFromBenchmark(value);
    }
    size_t JSON::GetChildrenCount(void) const {
        StartAndReturnFromBenchmark(children.GetSize());
    }
    bool JSON::AddChild(const JSON& child) {
        StartAndReturnFromBenchmark(children.Add(child));
    }
    Expected<JSON> JSON::Find(const Sequence<char>& path) const {
        StartBenchmark
        const Array<String> split = Split(path, "/"_M, false);
        JSON prev = *this;
        for (const Sequence<char>& name : split) {
            bool found = false;
            for (const JSON& child : prev.children) {
                if (child.name == name) {
                    prev = child;
                    found = true;
                    break;
                }
            }
            if (!found) ReturnFromBenchmark(Expected<JSON>());
        }
        ReturnFromBenchmark(prev);
    }
    Iterator<const JSON> JSON::begin(void) const {
        StartAndReturnFromBenchmark(children.begin());
    }
    Iterator<const JSON> JSON::end(void) const {
        StartAndReturnFromBenchmark(children.end());
    }
    Iterator<JSON> JSON::begin(void) {
        StartAndReturnFromBenchmark(children.begin());
    }
    Iterator<JSON> JSON::end(void) {
        StartAndReturnFromBenchmark(children.end());
    }
    bool JSON::Save(Writable& file) const {
        StartAndReturnFromBenchmark(file.Puts(ToString()));
    }
    bool JSON::Load(Readable& file) {
        StartBenchmark
        size_t i = 0;
        const Expected<JSON> tmp = LoadJSON(file.ReadUntil('\0'), i);
        if (!tmp.HasValue()) ReturnFromBenchmark(false);
        *this = tmp.Get();
        ReturnFromBenchmark(true);
    }
    String JSON::ToString(const Sequence<char>& padding) const {
        StartBenchmark
        String ret = CollectionToString(padding) + (name.IsEmpty() ? "" : '"'_M + name + "\": ");
        if (type == Type::String) ReturnFromBenchmark(ret + '"'_M + value + '"')
        else if (type == Type::Number || type == Type::Keyword) ReturnFromBenchmark(ret + value)
        else if (type == Type::Object || type == Type::Array) {
            ret += type == Type::Object ? "{\n" : "[\n";
            for (size_t i = 0; i < children.GetSize(); i++)
                ret += children.At(i).ToString(CollectionToString(padding) + '\t') + (i + 1 != children.GetSize() ? ",\n" : "\n");
            ReturnFromBenchmark(ret + padding + (type == Type::Object ? '}' : ']'));
        }
        else ReturnFromBenchmark(ret);
    }
    bool JSON::operator==(const JSON& other) const {
        StartAndReturnFromBenchmark(type == other.type && name == other.name && value == other.value && children == other.children);
    }
    bool JSON::operator!=(const JSON& other) const {
        StartAndReturnFromBenchmark(!(*this == other));
    }
}