#ifndef Target_H
#define Target_H
#include <Interfaces/Printable.hpp>

struct Target : MathLib::Printable {
    MathLib::String name;
    MathLib::String deps;
    MathLib::String command;
    bool phony;

    Target(const MathLib::Sequence<char>& name, const MathLib::Sequence<char>& deps, const MathLib::Sequence<char>& command, bool phony);
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
    [[nodiscard]] bool operator==(const Target& other) const;
    [[nodiscard]] bool operator!=(const Target& other) const;
};


#endif