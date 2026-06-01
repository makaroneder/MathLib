#ifndef MathLib_FileSystem_Cipher_H
#define MathLib_FileSystem_Cipher_H
#include "../../Interfaces/Sequence/ByteArray.hpp"
#include "../PhysicalFileSystem.hpp"
#include "FileCipher.hpp"
#include "CipherFile.hpp"

namespace MathLib {
    struct CipherFileSystem : PhysicalFileSystem {
        CipherFileSystem(ByteDevice& disk, FileCipher* cipher, const CipherKey& key);
        virtual ~CipherFileSystem(void) override;
        [[nodiscard]] virtual bool IsValid(void) const override;
        [[nodiscard]] virtual bool Create(void) override;
        [[nodiscard]] virtual size_t OpenInternal(const Sequence<char>& path, OpenMode mode) override;
        [[nodiscard]] virtual bool Close(size_t file) override;
        [[nodiscard]] virtual size_t Read(size_t file, void* buffer, size_t size, size_t position) override;
        [[nodiscard]] virtual size_t Write(size_t file, const void* buffer, size_t size, size_t position) override;
        [[nodiscard]] virtual size_t GetSize(size_t file) override;
        [[nodiscard]] virtual Array<FileInfo> ReadDirectory(const Sequence<char>& path) override;
        [[nodiscard]] virtual bool CreateDirectory(const Sequence<char>& path, bool overwrite) override;

        private:
        [[nodiscard]] size_t WriteInternal(size_t& newSize, ByteArray& data, const Sequence<char>& path, size_t prevSize, size_t writePosition, const void* buffer, size_t size, size_t position);

        CipherKey key;
        Array<CipherFile> files;
        FileCipher* cipher;
    };
}

#endif