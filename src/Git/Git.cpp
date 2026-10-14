#include "Git.hpp"
#include "GitTree.hpp"
#include "GitCommit.hpp"
#include <Cryptography/Compressor/ZLib.hpp>
#include <Cryptography/OneWayCipher/SHA/SHA1.hpp>

MathLib::String Git::HashToPath(const MathLib::String& hash) {
    return "objects/"_M + hash.AtUnsafe(0) + hash.AtUnsafe(1) + '/' + MathLib::SubString(hash, 2, hash.GetSize() - 2);
}
MathLib::String Git::GetDefaultReference(MathLib::FileSystem& fs) {
    MathLib::File file = fs.Open("HEAD"_M, MathLib::OpenMode::Read);
    MathLib::String str = file.ReadUntil('\n');
    if (!str.StartsWith("ref: refs/heads/"_M)) return "";
    return MathLib::SubString(str, 16, str.GetSize() - 16);
}
Git::Git(MathLib::Database& fs, const MathLib::String& reference) : reference(reference), fs(fs) {}
MathLib::String Git::GetRootCommit(void) const {
    MathLib::Array<uint8_t> file = fs.Read("refs/heads/"_M + reference);
    const size_t size = file.GetSize();
    size_t i = 0;
    MathLib::String ret;
    while (i < size) {
        char chr = file.AtUnsafe(i++);
        if (chr == '\n') break;
        ret += chr;
    }
    return ret.GetSize() == 40 ? ret : "";
}
MathLib::String Git::GetRootHash(void) const {
    const MathLib::String commitHash = GetRootCommit();
    if (commitHash.IsEmpty()) return "";
    GitCommit commit;
    return commit.LoadFromDatabase(fs, HashToPath(commitHash)) ? commit.treeHash : "";
}
MathLib::String Git::CreateBlob(const MathLib::Array<uint8_t>& data) {
    const MathLib::Array<uint8_t> blobData = GitBlob().Encrypt(data, MathLib::CipherKey());
    const MathLib::Array<uint8_t> hash = MathLib::SHA1().Encrypt(blobData, MathLib::CipherKey());
    const size_t size = hash.GetSize();
    MathLib::String ret = MathLib::ToString(hash.At(0), 16, 2);
    MathLib::String path = "objects/"_M + ret + '/';
    for (size_t i = 1; i < size; i++) {
        ret += MathLib::ToString(hash.AtUnsafe(i), 16, 2);
        path += MathLib::ToString(hash.AtUnsafe(i), 16, 2);
    }
    return fs.CreateRecord(path, MathLib::ZLib().Encrypt(blobData, MathLib::CipherKey())) ? ret : "";
}
bool Git::IsValid(void) const {
    return !GetRootHash().IsEmpty();
}
size_t Git::OpenInternal(const MathLib::Sequence<char>& path, MathLib::OpenMode mode) {
    MathLib::String prev = GetRootHash();
    const MathLib::Array<MathLib::String> split = Split(path, '/'_M, false);
    const size_t splitSize = split.GetSize();
    MathLib::Array<MathLib::String> hashes = MathLib::MakeArray<MathLib::String>(prev);
    bool create = false;
    for (size_t i = 0; i < splitSize; i++) {
        if (split.AtUnsafe(i).IsEmpty()) continue;
        GitTree tree;
        if (!tree.LoadFromDatabase(fs, HashToPath(prev))) return SIZE_MAX;
        bool found = false;
        for (const MathLib::DictionaryElement<MathLib::String, MathLib::String>& element : tree.elements) {
            if (element.GetKey() != split.AtUnsafe(i)) continue;
            prev = element.value;
            found = true;
            if (!hashes.Add(prev)) return SIZE_MAX;
            break;
        }
        if (found) continue;
        create = true;
        break;
    }
    if (mode ==  MathLib::OpenMode::Write || create) {
        if (mode == MathLib::OpenMode::Read) return SIZE_MAX;
        const size_t createFrom = hashes.GetSize();
        MathLib::String prev = CreateBlob(MathLib::Array<uint8_t>());
        for (size_t i = splitSize - 1; i > createFrom; i--) {
            GitTree tree;
            if (!tree.elements.Add(MathLib::DictionaryElement<MathLib::String, MathLib::String>(split.AtUnsafe(i), prev))) return SIZE_MAX;
            prev = tree.GetHash();
            if (!tree.SaveFromDatabase(fs, HashToPath(prev))) return SIZE_MAX;
        }
        for (size_t i = createFrom; i > !create; i--) {
            GitTree tree;
            if (!tree.LoadFromDatabase(fs, HashToPath(hashes.AtUnsafe(i - 1)))) return SIZE_MAX;
            if (!tree.elements.AddOrReplace(split.AtUnsafe(i), prev)) return SIZE_MAX;
            prev = tree.GetHash();
            if (!tree.SaveFromDatabase(fs, HashToPath(prev))) return SIZE_MAX;
        }
        GitCommit commit;
        commit.treeHash = prev;
        prev = commit.GetHash();
        if (!commit.SaveFromDatabase(fs, HashToPath(prev))) return SIZE_MAX;
        MathLib::Array<uint8_t> ret = prev.GetSize() + 1;
        for (size_t i = 0; i < prev.GetSize(); i++) ret.AtUnsafe(i) = prev.AtUnsafe(i);
        ret.AtUnsafe(prev.GetSize()) = '\n';
        if (!fs.CreateRecord("refs/heads/"_M + reference, ret)) return SIZE_MAX;
        return OpenInternal(path, MathLib::OpenMode::ReadWrite);
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
    const MathLib::Array<uint8_t> ret = GitBlob().DecryptPartial(MathLib::ZLib().Decrypt(fs.Read(HashToPath(files.AtUnsafe(file))), MathLib::CipherKey()), MathLib::CipherKey(), MathLib::Interval<size_t>(position, position + size));
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
    return GitBlob().Decrypt(MathLib::ZLib().Decrypt(fs.Read(HashToPath(files.AtUnsafe(file))), MathLib::CipherKey()), MathLib::CipherKey()).GetSize();
}
MathLib::Array<MathLib::FileInfo> Git::ReadDirectory(const MathLib::Sequence<char>& path) {
    MathLib::String prev = GetRootHash();
    const MathLib::Array<MathLib::String> split = Split(path, '/'_M, false);
    for (const MathLib::Sequence<char>& name : split) {
        if (name.IsEmpty()) continue;
        GitTree tree;
        if (!tree.LoadFromDatabase(fs, HashToPath(prev))) return MathLib::Array<MathLib::FileInfo>();
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
    if (!tree.LoadFromDatabase(fs, HashToPath(prev))) return MathLib::Array<MathLib::FileInfo>();
    const size_t size = tree.elements.GetSize();
    MathLib::Array<MathLib::FileInfo> ret = MathLib::Array<MathLib::FileInfo>(size);
    for (size_t i = 0; i < size; i++) {
        const MathLib::Array<uint8_t> id = MathLib::ZLib().DecryptPartial(fs.Read(HashToPath(tree.elements.AtUnsafe(i).value)), MathLib::CipherKey(), MathLib::Interval<size_t>(0, 5));
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