#ifndef MathLib_Interfaces_DictionaryElement_H
#define MathLib_Interfaces_DictionaryElement_H
#include "../Benchmark.hpp"

namespace MathLib {
    template <typename Key, typename Value>
    struct DictionaryElement {
        Value value;

        DictionaryElement(void) : value(), key() {
            EmptyBenchmark
        }
        DictionaryElement(const Key& key, const Value& value) : value(value), key(key) {
            EmptyBenchmark
        }
        [[nodiscard]] bool IsValidKey(const Key& k) const {
            StartAndReturnFromBenchmark(key == k);
        }
        [[nodiscard]] bool Replace(const DictionaryElement<Key, Value>& other) {
            StartBenchmark
            if (key != other.key) ReturnFromBenchmark(false);
            value = other.value;
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] Key GetKey(void) const {
            StartAndReturnFromBenchmark(key);
        }
        [[nodiscard]] bool operator==(const DictionaryElement<Key, Value>& other) const {
            StartAndReturnFromBenchmark(value == other.value);
        }
        [[nodiscard]] bool operator!=(const DictionaryElement<Key, Value>& other) const {
            StartAndReturnFromBenchmark(!(*this == other));
        }

        private:
        Key key;
    };
}

#endif