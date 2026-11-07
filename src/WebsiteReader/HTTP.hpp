#ifndef HTTP_H
#define HTTP_H
#include "Protocol.hpp"

struct HTTP : Protocol {
    [[nodiscard]] virtual MathLib::String GetScheme(void) const override;
    [[nodiscard]] virtual size_t GetDefaultPort(void) const override;
    [[nodiscard]] virtual MathLib::String Read(MathLib::RWDevice& socket, const MathLib::String& host, const MathLib::String& path) const override;
};

#endif