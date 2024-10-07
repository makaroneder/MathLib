#ifndef Math_Object_H
#define Math_Object_H
#define CreateOperators(self, T)            \
bool operator!=(const self& other) const {  \
    return !(*this == other);               \
}                                           \
self operator-(void) const {                \
    return *this * -1;                      \
}                                           \
self operator+(const self& other) const {   \
    self ret = *this;                       \
    ret.Add(other);                         \
    return ret;                             \
}                                           \
self operator-(const self& other) const {   \
    self ret = *this;                       \
    ret.Add(-other);                        \
    return ret;                             \
}                                           \
self operator*(const T& scalar) const {     \
    self ret = *this;                       \
    ret.Multiply(scalar);                   \
    return ret;                             \
}                                           \
self operator/(const T& scalar) const {     \
    self ret = *this;                       \
    ret.Multiply(1 / scalar);               \
    return ret;                             \
}                                           \
self operator+=(const self& other) {        \
    Add(other);                             \
    return *this;                           \
}                                           \
self operator-=(const self& other) {        \
    Add(-other);                            \
    return *this;                           \
}                                           \
self operator*=(const T& scalar) {          \
    Multiply(scalar);                       \
    return *this;                           \
}                                           \
self operator/=(const T& scalar) {          \
    Multiply(1 / scalar);                   \
    return *this;                           \
}
#endif