#include "ELFSectionHeader.hpp"
#include <String.hpp>

MathLib::String ELFSectionHeader::ToString(const MathLib::Sequence<char>& padding) const {
    MathLib::String ret = MathLib::CollectionToString(padding) + "{\n";
    ret += MathLib::CollectionToString(padding) + "\tName: 0x" + MathLib::ToString(name, 16) + '\n';
    ret += MathLib::CollectionToString(padding) + "\tType: 0x" + MathLib::ToString(type, 16) + '\n';
    ret += MathLib::CollectionToString(padding) + "\tFlags: 0x" + MathLib::ToString(flags, 16) + '\n';
    ret += MathLib::CollectionToString(padding) + "\tAddress: 0x" + MathLib::ToString(address, 16) + '\n';
    ret += MathLib::CollectionToString(padding) + "\tOffset: 0x" + MathLib::ToString(offset, 16) + '\n';
    ret += MathLib::CollectionToString(padding) + "\tSize: " + MathLib::ToString(size, 10) + '\n';
    ret += MathLib::CollectionToString(padding) + "\tLink: 0x" + MathLib::ToString(link, 16) + '\n';
    ret += MathLib::CollectionToString(padding) + "\tInfo: 0x" + MathLib::ToString(info, 16) + '\n';
    ret += MathLib::CollectionToString(padding) + "\tAlignment: " + MathLib::ToString(alignment, 10) + '\n';
    ret += MathLib::CollectionToString(padding) + "\tEntry size: " + MathLib::ToString(entrySize, 10) + '\n';
    return ret + padding + '}';
}