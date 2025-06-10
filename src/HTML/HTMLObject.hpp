#ifndef HTMLObject_H
#define HTMLObject_H
#include "HTMLAttribute.hpp"

struct HTMLObject : MathLib::Comparable, MathLib::Printable {
    enum class Type : uint8_t {
        Comment,
        Data,
        Element,
    };
    MathLib::String value;
    MathLib::Array<HTMLAttribute> attributes;
    MathLib::Array<HTMLObject> children;
    Type type;

    HTMLObject(void);
    HTMLObject(const MathLib::Sequence<char>& value, bool comment);
    HTMLObject(const MathLib::Sequence<char>& name, const MathLib::Sequence<HTMLAttribute>& attributes, const MathLib::Sequence<HTMLObject>& children);
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;

    protected:
    [[nodiscard]] virtual bool Equals(const MathLib::Comparable& other_) const override;
};
HTMLObject LoadHTMLObject(const MathLib::Sequence<char>& str, size_t& i);

#endif