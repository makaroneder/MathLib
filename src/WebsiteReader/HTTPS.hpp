#ifndef HTTPS_H
#define HTTPS_H
#include "TLSWrapper.hpp"
#include "HTTP.hpp"

struct HTTPS : TLSWrapper<HTTP> {
    [[nodiscard]] virtual MathLib::String GetScheme(void) const override;
    [[nodiscard]] virtual size_t GetDefaultPort(void) const override;
};

#endif