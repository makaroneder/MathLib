#include "CaesarCipher.hpp"

Expected<String> CaesarCipher::GetKey(const String& baseKey) const {
    return Expected<String>(String("1 ") + baseKey);
}