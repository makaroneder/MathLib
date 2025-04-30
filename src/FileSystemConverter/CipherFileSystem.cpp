#include "CipherFileSystem.hpp"
#include <ExternArray.hpp>
#include <Memory.hpp>

#include <iostream>

CipherFileSystem::CipherFileSystem(MathLib::ByteDevice& disk, FileCipher* cipher, const MathLib::Sequence<uint64_t>& key) : PhysicalFileSystem(disk), cipher(cipher), key(MathLib::CollectionToArray<uint64_t>(key)), files() {}
CipherFileSystem::~CipherFileSystem(void) {
    delete cipher;
}
bool CipherFileSystem::IsValid(void) const {
    if (!disk.Seek(0, MathLib::SeekMode::Set) || !cipher->IsValidHeader(disk, key)) return false;
    while (true) {
        const size_t pos = disk.Tell();
        const FileCipherData data = cipher->Identify(disk, key);
        if (data.type == FileCipherData::Type::Invalid) {
            if (!disk.Seek(pos, MathLib::SeekMode::Set)) return false;
            break;
        }
        if (!disk.Seek(pos + data.size, MathLib::SeekMode::Set)) return false;
    }
    return cipher->IsValidFooter(disk, key);
}
bool CipherFileSystem::Create(void) {
    return cipher->Create(disk, key);
}
size_t CipherFileSystem::OpenInternal(const MathLib::Sequence<char>& path, MathLib::OpenMode mode) {
    // TODO: Overwrite file data when opening with OpenMode::Write
    if (!disk.Seek(0, MathLib::SeekMode::Set)) return SIZE_MAX;
    const size_t size = disk.GetSize();
    while (true) {
        const size_t pos = disk.Tell();
        if (pos >= size) return SIZE_MAX;
        const FileCipherData data = cipher->Identify(disk, key);
        size_t size = SIZE_MAX;
        if (data.type == FileCipherData::Type::Invalid) {
            if (mode != MathLib::OpenMode::Write) return SIZE_MAX;
            MathLib::ByteArray data;
            uint8_t tmp;
            if (WriteInternal(size, data, path, 0, pos, &tmp, 0, 0)) return SIZE_MAX;
        }
        else if (data.type == FileCipherData::Type::Normal && data.name == path) size = data.size;
        if (size != SIZE_MAX) {
            const CipherFile ret = CipherFile(pos, size, path, mode);
            for (size_t i = 0; i < files.GetSize(); i++) {
                if (files.At(i).free) {
                    files.At(i) = ret;
                    return i;
                }
            }
            return files.Add(ret) ? files.GetSize() - 1 : SIZE_MAX;
        }
        else if (!disk.Seek(pos + data.size, MathLib::SeekMode::Set)) return SIZE_MAX;
    }
}
bool CipherFileSystem::Close(size_t file) {
    if (file >= files.GetSize() || files.At(file).free) return false;
    files.At(file).free = true;
    return true;
}
size_t CipherFileSystem::Read(size_t file, void* buffer, size_t size, size_t position) {
    if (file >= files.GetSize()) return 0;
    const CipherFile raw = files.At(file);
    if (!disk.Seek(raw.position, MathLib::SeekMode::Set)) return 0;
    const MathLib::Array<uint8_t> data = cipher->DecryptReadablePartial(disk, key, MathLib::Interval<size_t>(position, position + size));
    size = data.GetSize();
    MathLib::MemoryCopy(data.GetValue(), buffer, size);
    return size;
}
size_t CipherFileSystem::WriteInternal(size_t& newSize, MathLib::ByteArray& data, const MathLib::Sequence<char>& path, size_t prevSize, size_t writePosition, const void* buffer, size_t size, size_t position) {
    const size_t ret = data.WritePositionedSizedBuffer(buffer, size, position);
    MathLib::Array<uint64_t> tmpKey = key;
    const size_t pathSize = path.GetSize();
    char pathBuffer[pathSize + 1];
    for (size_t i = 0; i < pathSize; i++) pathBuffer[i] = path.At(i);
    pathBuffer[pathSize] = '\0';
    tmpKey += (uintptr_t)pathBuffer;
    const MathLib::Array<uint8_t> newData = cipher->Encrypt(data, tmpKey);
    if (newData.IsEmpty()) return 0;
    newSize = newData.GetSize();
    const size_t move = newSize - prevSize;
    if (move) {
        if (!IsValid()) return 0;
        const size_t remainingSize = disk.GetSizeLeft();
        if (!disk.Seek(0, MathLib::SeekMode::End)) return 0;
        for (size_t i = remainingSize; i <= move; i++)
            if (!disk.Write<uint8_t>(0)) return 0;
        const size_t end = writePosition + newSize;
        for (size_t i = disk.Tell() - move - 1; i > end; i--) {
            uint8_t tmp;
            if (!disk.ReadPositioned<uint8_t>(tmp, i) || !disk.WritePositioned<uint8_t>(tmp, i + move)) return 0;
        }
    }
    return disk.WritePositionedBuffer(newData.GetValue(), newSize, writePosition) && disk.Seek(0, MathLib::SeekMode::Set) && cipher->UpdateHeaderAndFooter(disk, key) ? ret : 0;
}
size_t CipherFileSystem::Write(size_t file, const void* buffer, size_t size, size_t position) {
    if (file >= files.GetSize()) return 0;
    const CipherFile raw = files.At(file);
    if (raw.mode == MathLib::OpenMode::Read) return 0;
    if (!disk.Seek(raw.position, MathLib::SeekMode::Set)) return 0;
    MathLib::ByteArray data = cipher->DecryptReadable(disk, key);
    size_t newSize;
    return WriteInternal(newSize, data, raw.path, raw.size, raw.position, buffer, size, position);
}
size_t CipherFileSystem::GetSize(size_t file) {
    if (file >= files.GetSize()) return 0;
    const CipherFile raw = files.At(file);
    return disk.Seek(raw.position, MathLib::SeekMode::Set) ? cipher->DecryptReadable(disk, key).GetSize() : 0;
}
MathLib::Array<MathLib::FileInfo> CipherFileSystem::ReadDirectory(const MathLib::Sequence<char>& path) {
    // TODO:
    (void)path;
    return MathLib::Array<MathLib::FileInfo>();
}