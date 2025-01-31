#ifndef DatabaseEntryField_H
#define DatabaseEntryField_H
#include <Interfaces/Saveable.hpp>
#include <Interfaces/Printable.hpp>

struct DatabaseEntryField : MathLib::Saveable, MathLib::Printable {
    DatabaseEntryField(void);
    DatabaseEntryField(const MathLib::String& name, const MathLib::String& value);
    MathLib::String GetName(void) const;
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
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::String& padding = "") const override;

    private:
    MathLib::String name;
    MathLib::String value;
};

#endif