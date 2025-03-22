#ifndef Database_H
#define Database_H
#include "DatabaseEntry.hpp"

struct Database : MathLib::Saveable, MathLib::Printable {
    static constexpr uint8_t expectedSignature = '!';

    Database(void);
    bool IsValid(void) const;
    MathLib::Expected<DatabaseEntry> GetEntry(const MathLib::Sequence<char>& name) const;
    bool UpdateEntry(const DatabaseEntry& entry);
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
    uint8_t signature;
    MathLib::Array<DatabaseEntry> entries;
};

#endif