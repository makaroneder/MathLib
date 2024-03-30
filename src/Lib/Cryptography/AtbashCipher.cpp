#include "AtbashCipher.hpp"

char AtbashCipher::EncryptChar(char chr, std::string key) const {
    return base.EncryptChar(chr, "-1 " + key);
}
char AtbashCipher::DecryptChar(char chr, std::string key) const {
    return base.DecryptChar(chr, "-1 " + key);
}