#ifndef TLSWrapper_H
#define TLSWrapper_H
#include "Protocol.hpp"
#include <TLS/TLS.hpp>

template <typename T>
struct TLSWrapper : Protocol {
    [[nodiscard]] virtual MathLib::String Read(MathLib::RWDevice& socket, const MathLib::String& host, const MathLib::String& path) const override {
        MathLib::TLS tls = socket;
        return tls.PerformHandshake(host) ? T().Read(tls, host, path) : "";
    }
};

#endif