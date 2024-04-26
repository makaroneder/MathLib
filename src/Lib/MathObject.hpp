#ifndef MathObject_H
#define MathObject_H
#define CreateOperators(self, T)    \
bool operator!=(self other) const { \
    return !(*this == other);       \
}                                   \
self operator-(void) const {        \
    return *this * -1;              \
}                                   \
self operator+(self other) const {  \
    self ret = *this;               \
    ret.Add(other);                 \
    return ret;                     \
}                                   \
self operator-(self other) const {  \
    self ret = *this;               \
    ret.Add(-other);                \
    return ret;                     \
}                                   \
self operator*(T scalar) const {    \
    self ret = *this;               \
    ret.Multiply(scalar);           \
    return ret;                     \
}                                   \
self operator/(T scalar) const {    \
    self ret = *this;               \
    ret.Multiply(1 / scalar);       \
    return ret;                     \
}                                   \
self operator+=(self other) {       \
    Add(other);                     \
    return *this;                   \
}                                   \
self operator-=(self other) {       \
    Add(-other);                    \
    return *this;                   \
}                                   \
self operator*=(T scalar) {         \
    Multiply(scalar);               \
    return *this;                   \
}                                   \
self operator/=(T scalar) {         \
    Multiply(1 / scalar);           \
    return *this;                   \
}
#endif