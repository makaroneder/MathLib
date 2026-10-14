#include "GitCommit.hpp"
#include "GitTree.hpp"
#include <Cryptography/Compressor/ZLib.hpp>
#include <Cryptography/OneWayCipher/SHA/SHA1.hpp>

MathLib::String GitCommit::GetHash(void) const {
    const MathLib::Array<uint8_t> hash = MathLib::SHA1().Encrypt(SaveInternal(), MathLib::CipherKey());
    const size_t size = hash.GetSize();
    MathLib::String ret;
    for (size_t i = 0; i < size; i++) ret += MathLib::ToString(hash.AtUnsafe(i), 16, 2);
    return ret;
}
MathLib::ByteArray GitCommit::SaveInternal(void) const {
    const size_t size = 5 + treeHash.GetSize() + 1;
    const MathLib::String sizeStr = MathLib::ToString(size, 10);
    MathLib::ByteArray buffer = size + 7 + 1 + sizeStr.GetSize();
    if (!buffer.Puts(commitStart) || !buffer.Puts(sizeStr) || !buffer.Write<char>('\0') || !buffer.Puts(GitTree::treeStart) || !buffer.Puts(treeHash) || !buffer.Write<char>('\n')) return MathLib::Array<uint8_t>();
    return buffer;
}
bool GitCommit::Save(MathLib::Writable& file) const {
    const MathLib::Array<uint8_t> data = MathLib::ZLib().Encrypt(SaveInternal(), MathLib::CipherKey());
    return file.WriteCollection<uint8_t>(data);
}
bool GitCommit::Load(MathLib::Readable& file) {
    const MathLib::Array<uint8_t> data = MathLib::ZLib().DecryptReadable(file, MathLib::CipherKey());
    size_t i = 7;
    if (!data.StartsWith(MathLib::ExternArray<uint8_t>((uint8_t*)commitStart, i))) return false;
    const size_t size = data.GetSize();
    MathLib::String sizeStr;
    while (true) {
        if (i >= size) return false;
        const char tmp = data.AtUnsafe(i++);
        if (!tmp) break;
        sizeStr += tmp;
    }
    if (MathLib::ToString(size - i, 10) != sizeStr) return false;
    if (!data.StartsWith(MathLib::ExternArray<uint8_t>((uint8_t*)GitTree::treeStart, 5), i)) return false;
    i += 5;
    if (!treeHash.Reset()) return false;
    while (true) {
        if (i >= size) return false;
        const char tmp = data.AtUnsafe(i++);
        if (tmp == '\n') break;
        treeHash += tmp;
    }
    return true;
}
MathLib::String GitCommit::ToString(const MathLib::Sequence<char>& padding) const {
    return MathLib::CollectionToString(padding) + treeHash;
}