#ifndef Dialog_H
#define Dialog_H
#include "DialogOption.hpp"

struct Dialog : MathLib::Allocatable {
    Dialog(const MathLib::String& text, const MathLib::Array<DialogOption>& options);
    template<typename T>
    [[nodiscard]] MathLib::Expected<size_t> GetNextDialog(const Player<T>& player, size_t option) {
        if (option >= options.GetSize()) return MathLib::Expected<size_t>();
        return MathLib::Expected<size_t>(options.At(option).GetNextDialog<T>(player));
    }
    template<typename T>
    [[nodiscard]] MathLib::String ToString(const Player<T>& player) const {
        MathLib::String ret;
        for (size_t i = 0; i < text.GetSize(); i++) ret += text.At(i) + '\n';
        for (size_t i = 0; i < options.GetSize(); i++) ret += MathLib::ToString(i) + ") " + options.At(i).ToString<T>(player) + '\n';
        return MathLib::SubString(ret, 0, ret.GetSize() - 1);
    }

    private:
    MathLib::Array<MathLib::String> text;
    MathLib::Array<DialogOption> options;
};

#endif