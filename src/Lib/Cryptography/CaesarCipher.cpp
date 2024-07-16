#include "CaesarCipher.hpp"

char CaesarCipher::EncryptChar(const char& chr, const String& key) const {
    return base.EncryptChar(chr, String("1 ") + key);
}
char CaesarCipher::DecryptChar(const char& chr, const String& key) const {
    return base.DecryptChar(chr, String("1 ") + key);
}