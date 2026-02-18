#include "ByteFiniteField.hpp"
#include "../String.hpp"

namespace MathLib {
    ByteFiniteField::ByteFiniteField(void) : value(0) {}
    ByteFiniteField::ByteFiniteField(uint8_t value) : value(value) {}
    ByteFiniteField ByteFiniteField::XTimes(void) const {
        uint8_t ret = 0;
        for (uint8_t i = 0; i < 7; i++)
            if (value & (1 << i)) ret |= 1 << (i + 1);
        return ret ^ (!!(value & (1 << 7)) * 0b00011011);
    }
    ByteFiniteField ByteFiniteField::operator+(const ByteFiniteField& other) const {
        return ByteFiniteField(value ^ other.value);
    }
    ByteFiniteField& ByteFiniteField::operator+=(const ByteFiniteField& other) {
        value ^= other.value;
        return *this;
    }
    ByteFiniteField ByteFiniteField::operator*(const ByteFiniteField& other) const {
        ByteFiniteField ret;
        for (uint8_t i = 0; i < 8; i++) {
            if (!(other.value & (1 << i))) continue;
            ByteFiniteField tmp = *this;
            for (uint8_t j = 0; j < i; j++) tmp = tmp.XTimes();
            ret.value ^= tmp.value;
        }
        return ret;
    }
    ByteFiniteField& ByteFiniteField::operator*=(const ByteFiniteField& other) {
        for (uint8_t i = 0; i < 8; i++) {
            if (!(other.value & (1 << i))) continue;
            ByteFiniteField tmp = *this;
            for (uint8_t j = 0; j < i; j++) tmp = tmp.XTimes();
            value ^= tmp.value;
        }
        return *this;
    }
    String ByteFiniteField::ToString(const Sequence<char>& padding) const {
        return CollectionToString(padding) + "0x" + MathLib::ToString(value, 16, 2); 
    }
    bool ByteFiniteField::Equals(const ByteFiniteField& other) const {
        return value == other.value;
    }
}