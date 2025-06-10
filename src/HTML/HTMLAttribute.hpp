#ifndef HTMLAttribute_H
#define HTMLAttribute_H
#include <Interfaces/Comparable.hpp>
#include <Interfaces/Printable.hpp>

struct HTMLAttribute : MathLib::Comparable, MathLib::Printable {
    MathLib::String name;
    MathLib::String value;

    HTMLAttribute(void);
    HTMLAttribute(const MathLib::Sequence<char>& name, const MathLib::Sequence<char>& value);
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;

    protected:
    [[nodiscard]] virtual bool Equals(const MathLib::Comparable& other_) const override;
};

#endif