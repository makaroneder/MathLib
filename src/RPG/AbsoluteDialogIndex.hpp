#ifndef AbsoluteDialogIndex_H
#define AbsoluteDialogIndex_H
#include <Interfaces/Saveable.hpp>
#include <Interfaces/Printable.hpp>

struct AbsoluteDialogIndex : Saveable, Printable {
    AbsoluteDialogIndex(void);
    AbsoluteDialogIndex(size_t npc, size_t dialog);
    /// @brief Saves data
    /// @param file File to save data into
    /// @return Status
    virtual bool Save(Writeable& file) const override;
    /// @brief Loads data
    /// @param file File to load data from
    /// @return Status
    virtual bool Load(Readable& file) override;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual String ToString(const String& padding = "") const override;
    bool operator==(const AbsoluteDialogIndex& other) const;

    private:
    size_t npc;
    size_t dialog;
};

#endif