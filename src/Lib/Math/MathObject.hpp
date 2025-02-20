#ifndef MathLib_Math_Object_H
#define MathLib_Math_Object_H
#define CreateOperators(self, T)                            \
[[nodiscard]] bool operator!=(const self& other) const {    \
    StartAndReturnFromBenchmark(!(*this == other));         \
}                                                           \
[[nodiscard]] self operator-(void) const {                  \
    StartAndReturnFromBenchmark(*this * -1);                \
}                                                           \
[[nodiscard]] self operator+(const self& other) const {     \
    StartBenchmark                                          \
    self ret = *this;                                       \
    ret.Add(other);                                         \
    ReturnFromBenchmark(ret);                               \
}                                                           \
[[nodiscard]] self operator-(const self& other) const {     \
    StartBenchmark                                          \
    self ret = *this;                                       \
    ret.Add(-other);                                        \
    ReturnFromBenchmark(ret);                               \
}                                                           \
[[nodiscard]] self operator*(const T& scalar) const {       \
    StartBenchmark                                          \
    self ret = *this;                                       \
    ret.Multiply(scalar);                                   \
    ReturnFromBenchmark(ret);                               \
}                                                           \
[[nodiscard]] self operator/(const T& scalar) const {       \
    StartBenchmark                                          \
    self ret = *this;                                       \
    ret.Multiply(1 / scalar);                               \
    ReturnFromBenchmark(ret);                               \
}                                                           \
self& operator+=(const self& other) {                       \
    StartBenchmark                                          \
    Add(other);                                             \
    ReturnFromBenchmark(*this);                             \
}                                                           \
self& operator-=(const self& other) {                       \
    StartBenchmark                                          \
    Add(-other);                                            \
    ReturnFromBenchmark(*this);                             \
}                                                           \
self& operator*=(const T& scalar) {                         \
    StartBenchmark                                          \
    Multiply(scalar);                                       \
    ReturnFromBenchmark(*this);                             \
}                                                           \
self& operator/=(const T& scalar) {                         \
    StartBenchmark                                          \
    Multiply(1 / scalar);                                   \
    ReturnFromBenchmark(*this);                             \
}
#endif