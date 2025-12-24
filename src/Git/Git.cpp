#include "Git.hpp"
#include "GitTree.hpp"
#include <Cryptography/OneWayCipher/SHA1.hpp>
#include <Cryptography/Compressor/ZLib.hpp>

[[nodiscard]] MathLib::String HashToPath(const MathLib::String& hash) {
    return "objects/"_M + hash.AtUnsafe(0) + hash.AtUnsafe(1) + '/' + MathLib::SubString(hash, 2, hash.GetSize() - 2);
}
Git::Git(MathLib::FileSystem& fs, const MathLib::String& rootHash) : rootHash(rootHash), fs(fs) {}
MathLib::String Git::CreateBlob(const MathLib::Array<uint8_t>& data) {
    const MathLib::Array<uint8_t> blobData = GitBlob().Encrypt(data, MathLib::Array<uint64_t>());
    const MathLib::Array<uint8_t> hash = MathLib::SHA1().Encrypt(blobData, MathLib::Array<uint64_t>());
    const size_t size = hash.GetSize();
    MathLib::String ret = MathLib::ToString(hash.At(0), 16, 2);
    MathLib::String path = "objects/"_M + ret + '/';
    if (!fs.CreateDirectory(path, false)) return "";
    for (size_t i = 1; i < size; i++) {
        ret += MathLib::ToString(hash.AtUnsafe(i), 16, 2);
        path += MathLib::ToString(hash.AtUnsafe(i), 16, 2);
    }
    const MathLib::Array<uint8_t> compressed = MathLib::ZLib().Encrypt(blobData, MathLib::Array<uint64_t>());
    MathLib::File file = fs.Open(path, MathLib::OpenMode::Write);
    if (!file.WriteBuffer(compressed.GetValue(), compressed.GetSize())) return "";
    return ret;
}
size_t Git::OpenInternal(const MathLib::Sequence<char>& path, MathLib::OpenMode mode) {
    // TODO: Create if needed
    (void)mode;
    MathLib::String prev = rootHash;
    const MathLib::Array<MathLib::String> split = Split(path, '/'_M, false);
    for (const MathLib::Sequence<char>& name : split) {
        if (name.IsEmpty()) continue;
        GitTree tree;
        if (!tree.LoadFromPath(fs, HashToPath(prev))) return SIZE_MAX;
        bool found = false;
        for (const MathLib::DictionaryElement<MathLib::String, MathLib::String>& element : tree.elements) {
            if (element.GetKey() == name) {
                prev = element.value;
                found = true;
                break;
            }
        }
        if (!found) return SIZE_MAX;
    }
    const size_t size = files.GetSize();
    for (size_t i = 0; i < size; i++) {
        if (files.AtUnsafe(i).IsEmpty()) {
            files.AtUnsafe(i) = prev;
            return i;
        }
    }
    return files.Add(prev) ? size : SIZE_MAX;
}
bool Git::Close(size_t file) {
    if (file >= files.GetSize() || files.AtUnsafe(file).IsEmpty()) return false;
    files.AtUnsafe(file) = "";
    return true;
}
size_t Git::Read(size_t file, void* buffer, size_t size, size_t position) {
    if (file >= files.GetSize() || files.AtUnsafe(file).IsEmpty()) return false;
    MathLib::File tmp = fs.Open(HashToPath(files.AtUnsafe(file)), MathLib::OpenMode::Read);
    const MathLib::Array<uint8_t> ret = GitBlob().DecryptPartial(MathLib::ZLib().DecryptReadable(tmp, MathLib::Array<uint64_t>()), MathLib::Array<uint64_t>(), MathLib::Interval<size_t>(position, position + size));
    const size_t retSize = ret.GetSize();
    MathLib::MemoryCopy(ret.GetValue(), buffer, retSize);
    return retSize;
}
size_t Git::Write(size_t file, const void* buffer, size_t size, size_t position) {
    // TODO:
    (void)file;
    (void)buffer;
    (void)size;
    (void)position;
    return 0;
}
size_t Git::GetSize(size_t file) {
    if (file >= files.GetSize() || files.AtUnsafe(file).IsEmpty()) return false;
    MathLib::File tmp = fs.Open(HashToPath(files.AtUnsafe(file)), MathLib::OpenMode::Read);
    return GitBlob().Decrypt(MathLib::ZLib().DecryptReadable(tmp, MathLib::Array<uint64_t>()), MathLib::Array<uint64_t>()).GetSize();
}
MathLib::Array<MathLib::FileInfo> Git::ReadDirectory(const MathLib::Sequence<char>& path) {
    MathLib::String prev = rootHash;
    const MathLib::Array<MathLib::String> split = Split(path, '/'_M, false);
    for (const MathLib::Sequence<char>& name : split) {
        if (name.IsEmpty()) continue;
        GitTree tree;
        if (!tree.LoadFromPath(fs, HashToPath(prev))) return MathLib::Array<MathLib::FileInfo>();
        bool found = false;
        for (const MathLib::DictionaryElement<MathLib::String, MathLib::String>& element : tree.elements) {
            if (element.GetKey() == name) {
                prev = element.value;
                found = true;
                break;
            }
        }
        if (!found) return MathLib::Array<MathLib::FileInfo>();
    }
    GitTree tree;
    if (!tree.LoadFromPath(fs, HashToPath(prev))) return MathLib::Array<MathLib::FileInfo>();
    const size_t size = tree.elements.GetSize();
    MathLib::Array<MathLib::FileInfo> ret = MathLib::Array<MathLib::FileInfo>(size);
    for (size_t i = 0; i < size; i++) {
        MathLib::File file = fs.Open(HashToPath(tree.elements.AtUnsafe(i).value), MathLib::OpenMode::Read);
        const MathLib::Array<uint8_t> id = MathLib::ZLib().DecryptReadablePartial(file, MathLib::Array<uint64_t>(), MathLib::Interval<size_t>(0, 5));
        MathLib::FileInfo::Type type = MathLib::FileInfo::Type::Unknown;
        if (id == MathLib::MakeArray<uint8_t>('b', 'l', 'o', 'b', ' ')) type = MathLib::FileInfo::Type::File;
        if (id == MathLib::MakeArray<uint8_t>('t', 'r', 'e', 'e', ' ')) type = MathLib::FileInfo::Type::Directory;
        ret.AtUnsafe(i) = MathLib::FileInfo(type, tree.elements.AtUnsafe(i).GetKey());
    }
    return ret;
}
bool Git::CreateDirectory(const MathLib::Sequence<char>& path, bool overwrite) {
    // TODO:
    (void)path;
    (void)overwrite;
    return false;
}