#ifndef MathLib_Interfaces_DictionaryElement_H
#define MathLib_Interfaces_DictionaryElement_H
#include "Printable.hpp"

namespace MathLib {
    template <typename Key, typename Value>
    struct DictionaryElement : Printable {
        Value value;

        DictionaryElement(void) : value(), key() {}
        DictionaryElement(const Key& key, const Value& value) : value(value), key(key) {}
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override {
            return Formatter<Key>::ToString(key, padding) + ": " + Formatter<Value>::ToString(value);
        }
        [[nodiscard]] bool IsValidKey(const Key& k) const {
            return key == k;
        }
        [[nodiscard]] bool Replace(const DictionaryElement<Key, Value>& other) {
            if (key != other.key) return false;
            value = other.value;
            return true;
        }
        [[nodiscard]] Key GetKey(void) const {
            return key;
        }
        [[nodiscard]] bool operator==(const DictionaryElement<Key, Value>& other) const {
            return value == other.value;
        }
        [[nodiscard]] bool operator!=(const DictionaryElement<Key, Value>& other) const {
            return !(*this == other);
        }

        private:
        Key key;
    };
}

#endif