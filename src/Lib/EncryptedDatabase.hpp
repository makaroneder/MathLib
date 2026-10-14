#ifndef MathLib_EncryptedDatabase_H
#define MathLib_EncryptedDatabase_H
#include "Database.hpp"
#include "Cryptography/ReadableCipher.hpp"

namespace MathLib {
    struct EncryptedDatabase : Database {
        EncryptedDatabase(Database& base, const ReadableCipher& cipher, const CipherKey& key);
        [[nodiscard]] virtual bool CreateRecord(const Sequence<char>& path, const Sequence<uint8_t>& data) override;
        [[nodiscard]] virtual Array<uint8_t> Read(const Sequence<char>& path) override;
        [[nodiscard]] virtual bool Overwrite(const Sequence<char>& path, const void* buffer, size_t size, size_t position) override;

        private:
        CipherKey key;
        Database& base;
        const ReadableCipher& cipher;
    };
}

#endif