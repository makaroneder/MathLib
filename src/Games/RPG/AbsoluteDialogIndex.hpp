#ifndef AbsoluteDialogIndex_H
#define AbsoluteDialogIndex_H
#include <Interfaces/Saveable.hpp>
#include <Interfaces/Printable.hpp>

struct AbsoluteDialogIndex : MathLib::Saveable, MathLib::Printable {
    AbsoluteDialogIndex(void);
    AbsoluteDialogIndex(size_t npc, size_t dialog);
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
    [[nodiscard]] bool operator==(const AbsoluteDialogIndex& other) const;

    private:
    size_t npc;
    size_t dialog;
};

#endif