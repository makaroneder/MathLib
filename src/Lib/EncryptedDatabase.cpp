#include "EncryptedDatabase.hpp"

namespace MathLib {
    EncryptedDatabase::EncryptedDatabase(Database& base, const ReadableCipher& cipher, const CipherKey& key) : key(key), base(base), cipher(cipher) {}
    bool EncryptedDatabase::CreateRecord(const Sequence<char>& path, const Sequence<uint8_t>& data) {
        return base.CreateRecord(path, cipher.Encrypt(data, key));
    }
    Array<uint8_t> EncryptedDatabase::Read(const Sequence<char>& path) {
        return cipher.Decrypt(base.Read(path), key);
    }
    bool EncryptedDatabase::Overwrite(const Sequence<char>& path, const void* buffer, size_t size, size_t position) {
        return DefaultOverwrite(path, buffer, size, position);
    }
}