#ifndef Dialog_H
#define Dialog_H
#include "DialogOption.hpp"

struct Dialog {
    Dialog(const String& text, const Array<DialogOption>& options);
    template<typename T>
    Expected<size_t> GetNextDialog(const Player<T>& player, size_t option) {
        if (option >= options.GetSize()) return Expected<size_t>();
        return Expected<size_t>(options.At(option).GetNextDialog<T>(player));
    }
    template<typename T>
    String ToString(const Player<T>& player) const {
        String ret;
        for (size_t i = 0; i < text.GetSize(); i++) ret += text.At(i) + '\n';
        for (size_t i = 0; i < options.GetSize(); i++) ret += ::ToString(i) + ") " + options.At(i).ToString<T>(player) + '\n';
        return SubString(ret, 0, ret.GetSize() - 1);
    }

    private:
    Array<String> text;
    Array<DialogOption> options;
};

#endif