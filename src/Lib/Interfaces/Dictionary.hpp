#ifndef MathLib_Interfaces_Dictionary_H
#define MathLib_Interfaces_Dictionary_H
#include "DictionaryElement.hpp"
#include "../Expected.hpp"

namespace MathLib {
    template <typename Key, typename Value>
    struct Dictionary : Collection<DictionaryElement<Key, Value>> {
        Dictionary(void) {
            EmptyBenchmark
        }
        Dictionary(size_t size) : elements(size) {
            EmptyBenchmark
        }
        Dictionary(const Sequence<DictionaryElement<Key, Value>>& elements) : elements(CollectionToArray<DictionaryElement<Key, Value>>(elements)) {
            EmptyBenchmark
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            StartAndReturnFromBenchmark(elements.GetSize());
        }
        [[nodiscard]] virtual DictionaryElement<Key, Value>* GetValue(void) override {
            StartAndReturnFromBenchmark(elements.GetValue());
        }
        [[nodiscard]] virtual const DictionaryElement<Key, Value>* GetValue(void) const override {
            StartAndReturnFromBenchmark(elements.GetValue());
        }
        [[nodiscard]] virtual bool Add(const DictionaryElement<Key, Value>& value) override {
            StartAndReturnFromBenchmark(elements.Add(value));
        }
        [[nodiscard]] virtual bool Reset(void) override {
            StartAndReturnFromBenchmark(elements.Reset());
        }
        [[nodiscard]] Expected<Value> Get(const Key& key) const {
            StartBenchmark
            for (const DictionaryElement<Key, Value>& element : elements)
                if (element.IsValidKey(key)) ReturnFromBenchmark(element.value);
            ReturnFromBenchmark(Expected<Value>());
        }
        [[nodiscard]] bool AddOrReplace(const DictionaryElement<Key, Value>& element) {
            StartBenchmark
            for (DictionaryElement<Key, Value>& x : elements)
                if (x.Replace(element)) ReturnFromBenchmark(true);
            ReturnFromBenchmark(elements.Add(element));
        }
        [[nodiscard]] bool AddOrModify(const Key& key, const Function<Value, Value>& function, const Value& defaultValue) {
            StartBenchmark
            for (DictionaryElement<Key, Value>& element : elements) {
                if (element.IsValidKey(key)) {
                    element.value = function(element.value);
                    ReturnFromBenchmark(true);
                }
            }
            ReturnFromBenchmark(elements.Add(DictionaryElement<Key, Value>(key, defaultValue)));
        }

        private:
        Array<DictionaryElement<Key, Value>> elements;
    };
}

#endif