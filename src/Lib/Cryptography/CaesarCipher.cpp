#include "CaesarCipher.hpp"

char CaesarCipher::EncryptChar(char chr, String key) const {
    return base.EncryptChar(chr, String("1 ") + key);
}
char CaesarCipher::DecryptChar(char chr, String key) const {
    return base.DecryptChar(chr, String("1 ") + key);
}