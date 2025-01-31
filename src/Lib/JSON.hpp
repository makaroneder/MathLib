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
        JSON(Type type, const String& name, const String& value);
        [[nodiscard]] Type GetType(void) const;
        [[nodiscard]] String GetValue(void) const;
        [[nodiscard]] size_t GetChildrenCount(void) const;
        [[nodiscard]] bool AddChild(const JSON& child);
        [[nodiscard]] Expected<JSON> Find(const String& path) const;
        [[nodiscard]] virtual Iterator<const JSON> begin(void) const override;
        [[nodiscard]] virtual Iterator<const JSON> end(void) const override;
        [[nodiscard]] virtual Iterator<JSON> begin(void) override;
        [[nodiscard]] virtual Iterator<JSON> end(void) override;
        /// @brief Saves data
        /// @param file File to save data into
        /// @return Status
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        /// @brief Loads data
        /// @param file File to load data from
        /// @return Status
        [[nodiscard]] virtual bool Load(Readable& file) override;
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const String& padding = "") const override;

        private:
        Type type;
        String name;
        String value;
        Array<JSON> children;
    };
}

#endif