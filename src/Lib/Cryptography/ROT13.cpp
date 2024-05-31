#include "ROT13.hpp"

char ROT13::EncryptChar(char chr, String key) const {
    if (!key.IsEmpty()) return INT8_MAX;
    return base.EncryptChar(chr, "13");
}
char ROT13::DecryptChar(char chr, String key) const {
    if (!key.IsEmpty()) return INT8_MAX;
    return base.DecryptChar(chr, "13");
}