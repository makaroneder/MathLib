#ifndef MathLib_JSON_H
#define MathLib_JSON_H
#include "Interfaces/Printable.hpp"
#include "Interfaces/Saveable.hpp"

namespace MathLib {
    struct JSON : Printable, Saveable, Iteratable<JSON> {
        enum class Type {
            Keyword,
            Number,
            String,
            Object,
            Array,
        };
        JSON(void);
        JSON(Type type, const Sequence<char>& name, const Sequence<char>& value);
        [[nodiscard]] Type GetType(void) const;
        [[nodiscard]] String GetValue(void) const;
        [[nodiscard]] size_t GetChildrenCount(void) const;
        [[nodiscard]] bool AddChild(const JSON& child);
        [[nodiscard]] Expected<JSON> GetChild(const Sequence<char>& name) const;
        [[nodiscard]] Expected<JSON> Find(const Sequence<char>& path) const;
        [[nodiscard]] virtual Iterator<const JSON> begin(void) const override;
        [[nodiscard]] virtual Iterator<const JSON> end(void) const override;
        [[nodiscard]] virtual Iterator<JSON> begin(void) override;
        [[nodiscard]] virtual Iterator<JSON> end(void) override;
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
        [[nodiscard]] bool operator==(const JSON& other) const;
        [[nodiscard]] bool operator!=(const JSON& other) const;

        private:
        String name;
        String value;
        Array<JSON> children;
        Type type;
    };
}

#endif