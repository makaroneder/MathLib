#include "AtbashCipher.hpp"

char AtbashCipher::EncryptChar(char chr, String key) const {
    return base.EncryptChar(chr, String("-1 ") + key);
}
char AtbashCipher::DecryptChar(char chr, String key) const {
    return base.DecryptChar(chr, String("-1 ") + key);
}