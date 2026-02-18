#include "GitTree.hpp"
#include <Interfaces/Sequence/ByteArray.hpp>
#include <Cryptography/Compressor/ZLib.hpp>
#include <ExternArray.hpp>

bool GitTree::Save(MathLib::Writable& file) const {
    const size_t elementCount = elements.GetSize();
    size_t size = elementCount * (7 + 1 + 20);
    for (const MathLib::DictionaryElement<MathLib::String, MathLib::String>& element : elements)
        size += element.GetKey().GetSize();
    const MathLib::String sizeStr = MathLib::ToString(size, 10);
    MathLib::ByteArray buffer = MathLib::ByteArray(5 + sizeStr.GetSize() + 1 + size);
    if (!buffer.WriteBuffer(treeStart, 5)) return false;
    if (!buffer.Puts(sizeStr) || !buffer.Write<char>('\0')) return false;
    for (const MathLib::DictionaryElement<MathLib::String, MathLib::String>& element : elements) {
        if (!buffer.WriteBuffer(fileMode, 7) || !buffer.Puts(element.GetKey()) || !buffer.Write<char>('\0')) return false;
        const size_t size = element.value.GetSize();
        for (size_t i = 0; i < size; i += 2)
            if (!buffer.Write<uint8_t>(MathLib::StringToU8(element.value.AtUnsafe(i), element.value.AtUnsafe(i + 1)))) return false;
    }
    const MathLib::Array<uint8_t> data = MathLib::ZLib().Encrypt(buffer, MathLib::CipherKey());
    return file.WriteBuffer(data.GetValue(), data.GetSize());
}
bool GitTree::Load(MathLib::Readable& file) {
    const MathLib::Array<uint8_t> data = MathLib::ZLib().DecryptReadable(file, MathLib::CipherKey());
    size_t i = 5;
    if (!data.StartsWith(MathLib::ExternArray<uint8_t>((uint8_t*)treeStart, i))) return false;
    const size_t size = data.GetSize();
    MathLib::String sizeStr;
    while (true) {
        if (i >= size) return false;
        const char tmp = data.AtUnsafe(i++);
        if (!tmp) break;
        sizeStr += tmp;
    }
    if (MathLib::ToString(size - i, 10) != sizeStr) return false;
    while (i < size) {
        while (true) {
            const char tmp = data.AtUnsafe(i++);
            if (tmp == ' ') break;
        }
        MathLib::String name;
        while (true) {
            const char tmp = data.AtUnsafe(i++);
            if (!tmp) break;
            name += tmp;
        }
        MathLib::String hash;
        for (uint8_t j = 0; j < 20; j++) hash += MathLib::ToString(data.AtUnsafe(i++), 16, 2);
        if (!elements.Add(name, hash)) return false;
    }
    return true;
}
MathLib::String GitTree::ToString(const MathLib::Sequence<char>& padding) const {
    const MathLib::String padd = MathLib::CollectionToString(padding);
    const MathLib::String padd2 = padd + '\t';
    MathLib::String ret = "{\n";
    for (const MathLib::DictionaryElement<MathLib::String, MathLib::String>& element : elements)
        ret += element.ToString(padd2);
    return ret + padd + '}';
}