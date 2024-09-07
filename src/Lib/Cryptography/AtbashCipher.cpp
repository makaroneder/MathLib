#include "AtbashCipher.hpp"

Expected<String> AtbashCipher::GetKey(const String& baseKey) const {
    return Expected<String>(String("-1 ") + baseKey);
}