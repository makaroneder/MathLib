#ifndef DatabaseEntry_H
#define DatabaseEntry_H
#include "DatabaseEntryField.hpp"

struct DatabaseEntry : MathLib::Saveable, MathLib::Printable {
    DatabaseEntry(const MathLib::Sequence<char>& name = ""_M);
    MathLib::String GetName(void) const;
    MathLib::Expected<DatabaseEntryField> GetField(const MathLib::Sequence<char>& name) const;
    bool UpdateField(const DatabaseEntryField& field);
    /// @brief Saves data
    /// @param file File to save data into
    /// @return Status
    [[nodiscard]] virtual bool Save(MathLib::Writable& file) const override;
    /// @brief Loads data
    /// @param file File to load data from
    /// @return Status
    [[nodiscard]] virtual bool Load(MathLib::Readable& file) override;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;

    private:
    MathLib::String name;
    MathLib::Array<DatabaseEntryField> fields;
};

#endif