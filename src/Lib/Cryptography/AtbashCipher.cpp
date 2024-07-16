#include "AtbashCipher.hpp"

char AtbashCipher::EncryptChar(const char& chr, const String& key) const {
    return base.EncryptChar(chr, String("-1 ") + key);
}
char AtbashCipher::DecryptChar(const char& chr, const String& key) const {
    return base.DecryptChar(chr, String("-1 ") + key);
}