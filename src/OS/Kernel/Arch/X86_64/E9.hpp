#ifdef __x86_64__
#ifndef E9_H
#define E9_H
#include <Interfaces/Writable.hpp>

struct E9 : MathLib::Writable {
    [[nodiscard]] static bool IsPresent(void);
    [[nodiscard]] virtual size_t WriteSizedBuffer(const void* buffer, size_t size) override;
};

#endif
#endif