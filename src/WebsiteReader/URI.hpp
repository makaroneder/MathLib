#ifndef URI_H
#define URI_H
#include <Interfaces/Printable.hpp>

struct URI : MathLib::Printable {
    MathLib::String scheme;
    MathLib::String userInfo;
    MathLib::String host;
    MathLib::String path;
    MathLib::String query;
    MathLib::String fragment;
    size_t port;

    URI(void);
    URI(const MathLib::Sequence<char>& str);
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
};

#endif