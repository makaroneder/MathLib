#ifndef MathLib_Interfaces_Dictionary_H
#define MathLib_Interfaces_Dictionary_H
#include "DictionaryElement.hpp"
#include "../Expected.hpp"

namespace MathLib {
    template <typename Key, typename Value>
    struct Dictionary : Collection<DictionaryElement<Key, Value>>, Printable {
        Dictionary(void) {}
        Dictionary(size_t size) : elements(size) {}
        Dictionary(const Sequence<DictionaryElement<Key, Value>>& elements) : elements(CollectionToArray<DictionaryElement<Key, Value>>(elements)) {}
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return elements.GetSize();
        }
        [[nodiscard]] virtual DictionaryElement<Key, Value>* GetValue(void) override {
            return elements.GetValue();
        }
        [[nodiscard]] virtual const DictionaryElement<Key, Value>* GetValue(void) const override {
            return elements.GetValue();
        }
        [[nodiscard]] virtual bool Add(const DictionaryElement<Key, Value>& value) override {
            return elements.Add(value);
        }
        [[nodiscard]] virtual bool Reset(void) override {
            return elements.Reset();
        }
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override {
            const String padd = CollectionToString(padding);
            const String padd2 = padd + '\t';
            String ret = "{\n";
            for (const DictionaryElement<Key, Value>& element : elements) ret += element.ToString(padd2) + '\n';
            return ret + padd + '}';
        }
        [[nodiscard]] bool ContainsKey(const Key& key) const {
            for (const DictionaryElement<Key, Value>& element : elements)
                if (element.IsValidKey(key)) return true;
            return false;
        }
        [[nodiscard]] bool ContainsValue(const Value& value) const {
            for (const DictionaryElement<Key, Value>& element : elements)
                if (element.value == value) return true;
            return false;
        }
        void FillValues(const Value& value) {
            for (DictionaryElement<Key, Value>& element : elements) element.value = value;
        }
        [[nodiscard]] Expected<Value> Get(const Key& key) const {
            for (const DictionaryElement<Key, Value>& element : elements)
                if (element.IsValidKey(key)) return element.value;
            return Expected<Value>();
        }
        [[nodiscard]] bool Add(const Key& key, const Value& value) {
            return elements.Add(DictionaryElement<Key, Value>(key, value));
        }
        [[nodiscard]] bool AddOrReplace(const DictionaryElement<Key, Value>& element) {
            for (DictionaryElement<Key, Value>& x : elements)
                if (x.Replace(element)) return true;
            return elements.Add(element);
        }
        [[nodiscard]] bool AddOrModify(const Key& key, const Function<Value, Value>& function, const Value& defaultValue) {
            for (DictionaryElement<Key, Value>& element : elements) {
                if (!element.IsValidKey(key)) continue;
                element.value = function(element.value);
                return true;
            }
            return elements.Add(DictionaryElement<Key, Value>(key, defaultValue));
        }
        void Map(const Function<Value, Value>& function) {
            for (DictionaryElement<Key, Value>& a : elements) a.value = function(a.value);
        }
        template <typename T>
        [[nodiscard]] bool Combine(const Dictionary<Key, T>& other, const Function<Value, Value, T>& function, const Value& defaultValue) {
            for (const DictionaryElement<Key, T>& b : other) {
                const Key key = b.GetKey();
                bool found = false;
                for (DictionaryElement<Key, Value>& a : elements) {
                    if (!a.IsValidKey(key)) continue;
                    a.value = function(a.value, b.value);
                    found = true;
                    break;
                }
                if (!found && !elements.Add(DictionaryElement<Key, Value>(key, function(defaultValue, b.value)))) return false;
            }
            return true;
        }

        private:
        Array<DictionaryElement<Key, Value>> elements;
    };
}

#endif