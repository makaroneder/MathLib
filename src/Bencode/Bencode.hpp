#ifndef Bencode_H
#define Bencode_H
#include <Interfaces/Saveable.hpp>

struct Bencode;
struct Bencode : MathLib::Comparable<Bencode>, MathLib::Saveable, MathLib::Printable {
    enum class Type : uint8_t {
        End,
        Number,
        String,
        List,
        Dictionary,
    };
    MathLib::String value;
    MathLib::Array<Bencode> children;
    Type type;

    Bencode(void);
    Bencode(const MathLib::Sequence<char>& value, bool number);
    Bencode(const MathLib::Sequence<Bencode>& children, bool list);
    [[nodiscard]] virtual bool Save(MathLib::Writable& file) const override;
    [[nodiscard]] virtual bool Load(MathLib::Readable& file) override;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;

    protected:
    [[nodiscard]] virtual bool Equals(const Bencode& other) const override;
};

#endif