#include "CaesarCipher.hpp"

char CaesarCipher::EncryptChar(char chr, std::string key) const {
    return base.EncryptChar(chr, "1 " + key);
}
char CaesarCipher::DecryptChar(char chr, std::string key) const {
    return base.DecryptChar(chr, "1 " + key);
}