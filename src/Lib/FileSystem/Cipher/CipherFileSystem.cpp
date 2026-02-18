#include "../../ExternArray.hpp"
#include "CipherFileSystem.hpp"
#include "../../Memory.hpp"
#include "../Path.hpp"

namespace MathLib {
    CipherFileSystem::CipherFileSystem(ByteDevice& disk, FileCipher* cipher, const CipherKey& key) : PhysicalFileSystem(disk), key(key), files(), cipher(cipher) {}
    CipherFileSystem::~CipherFileSystem(void) {
        delete cipher;
    }
    bool CipherFileSystem::IsValid(void) const {
        if (!disk.Seek(0, SeekMode::Set) || !cipher->IsValidHeader(disk, key)) return false;
        while (true) {
            const size_t pos = disk.Tell();
            const FileCipherData data = cipher->Identify(disk, key);
            if (data.type == FileCipherData::Type::Invalid) {
                if (!disk.Seek(pos, SeekMode::Set)) return false;
                break;
            }
            if (!disk.Seek(pos + data.size, SeekMode::Set)) return false;
        }
        return cipher->IsValidFooter(disk, key);
    }
    bool CipherFileSystem::Create(void) {
        return disk.Seek(0, SeekMode::Set) && cipher->Create(disk, key);
    }
    size_t CipherFileSystem::OpenInternal(const Sequence<char>& path, OpenMode mode) {
        // TODO: Overwrite file data when opening with OpenMode::Write
        if (!disk.Seek(0, SeekMode::Set) || !cipher->IsValidHeader(disk, key)) return SIZE_MAX;
        const size_t size = disk.GetSize();
        while (true) {
            const size_t pos = disk.Tell();
            if (pos > size) return SIZE_MAX;
            const FileCipherData data = pos < size ? cipher->Identify(disk, key) : FileCipherData();
            size_t size = SIZE_MAX;
            if (data.type == FileCipherData::Type::Invalid) {
                if (mode != OpenMode::Write) return SIZE_MAX;
                ByteArray data;
                uint8_t tmp;
                if (WriteInternal(size, data, path, 0, pos, &tmp, 0, 0)) return SIZE_MAX;
            }
            else if (data.type == FileCipherData::Type::Normal && data.name == path) size = data.size;
            if (size != SIZE_MAX) {
                const CipherFile ret = CipherFile(pos, size, path, mode);
                const size_t size = files.GetSize();
                for (size_t i = 0; i < size; i++) {
                    if (files.At(i).free) {
                        files.At(i) = ret;
                        return i;
                    }
                }
                return files.Add(ret) ? size : SIZE_MAX;
            }
            if (!disk.Seek(pos + data.size, SeekMode::Set)) return SIZE_MAX;
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
        if (!disk.Seek(raw.position, SeekMode::Set)) return 0;
        const Array<uint8_t> data = cipher->DecryptReadablePartial(disk, key, Interval<size_t>(position, position + size));
        size = data.GetSize();
        MemoryCopy(data.GetValue(), buffer, size);
        return size;
    }
    size_t CipherFileSystem::WriteInternal(size_t& newSize, ByteArray& data, const Sequence<char>& path, size_t prevSize, size_t writePosition, const void* buffer, size_t size, size_t position) {
        const size_t ret = data.WritePositionedSizedBuffer(buffer, size, position);
        cipher->path = CollectionToString(path);
        const Array<uint8_t> newData = cipher->Encrypt(data, key);
        if (newData.IsEmpty()) return 0;
        newSize = newData.GetSize();
        const size_t move = newSize - prevSize;
        if (move) {
            if (!IsValid()) return 0;
            const size_t remainingSize = disk.GetSizeLeft();
            if (!disk.Seek(0, SeekMode::End)) return 0;
            for (size_t i = remainingSize; i <= move; i++)
                if (!disk.Write<uint8_t>(0)) return 0;
            const size_t end = writePosition + newSize;
            for (size_t i = disk.Tell() - move - 1; i > end; i--) {
                uint8_t tmp;
                if (!disk.ReadPositioned<uint8_t>(tmp, i) || !disk.WritePositioned<uint8_t>(tmp, i + move)) return 0;
            }
        }
        return disk.WritePositionedBuffer(newData.GetValue(), newSize, writePosition) && disk.Seek(0, SeekMode::Set) && cipher->UpdateHeaderAndFooter(disk, key) ? ret : 0;
    }
    size_t CipherFileSystem::Write(size_t file, const void* buffer, size_t size, size_t position) {
        if (file >= files.GetSize()) return 0;
        const CipherFile raw = files.At(file);
        if (raw.mode == OpenMode::Read || !disk.Seek(raw.position, SeekMode::Set)) return 0;
        ByteArray data = cipher->DecryptReadable(disk, key);
        size_t newSize;
        return WriteInternal(newSize, data, raw.path, raw.size, raw.position, buffer, size, position);
    }
    size_t CipherFileSystem::GetSize(size_t file) {
        if (file >= files.GetSize()) return 0;
        const CipherFile raw = files.At(file);
        return disk.Seek(raw.position, SeekMode::Set) ? cipher->DecryptReadable(disk, key).GetSize() : 0;
    }
    Array<FileInfo> CipherFileSystem::ReadDirectory(const Sequence<char>& path) {
        if (!disk.Seek(0, SeekMode::Set) || !cipher->IsValidHeader(disk, key)) return Array<FileInfo>();
        Array<FileInfo> ret;
        while (true) {
            const size_t pos = disk.Tell();
            const FileCipherData data = cipher->Identify(disk, key);
            if (data.type == FileCipherData::Type::Invalid) return ret;
            if (data.type == FileCipherData::Type::Normal) {
                const SingleTypePair<String> name = RemoveBasePathAndPopFirstPathElement(path, data.name);
                if (!name.first.IsEmpty()) {
                    const FileInfo info = FileInfo(name.second.IsEmpty() ? FileInfo::Type::File : FileInfo::Type::Directory, name.first);
                    if (!(ret.Contains(info) || ret.Add(info))) return Array<FileInfo>();
                }
            }
            if (!disk.Seek(pos + data.size, SeekMode::Set)) return Array<FileInfo>();
        }
    }
    bool CipherFileSystem::CreateDirectory(const Sequence<char>&, bool) {
        // TODO:
        return false;
    }
}