#include "HTTPS.hpp"

MathLib::String HTTPS::GetScheme(void) const {
    return "https";
}
size_t HTTPS::GetDefaultPort(void) const {
    return 443;
}