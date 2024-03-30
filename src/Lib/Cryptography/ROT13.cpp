#include "ROT13.hpp"

char ROT13::EncryptChar(char chr, std::string key) const {
    if (!key.empty()) return INT8_MAX;
    return base.EncryptChar(chr, "13");
}
char ROT13::DecryptChar(char chr, std::string key) const {
    if (!key.empty()) return INT8_MAX;
    return base.DecryptChar(chr, "13");
}