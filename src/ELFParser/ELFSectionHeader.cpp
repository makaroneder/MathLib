#include "ELFSectionHeader.hpp"
#include <String.hpp>

MathLib::String ELFSectionHeader::ToString(const MathLib::Sequence<char>& padding) const {
    return MathLib::CollectionToString(padding) + "{\n" +
        padding + "\tName: 0x" + MathLib::ToString(name, 16) + '\n' +
        padding + "\tType: 0x" + MathLib::ToString(type, 16) + '\n' +
        padding + "\tFlags: 0x" + MathLib::ToString(flags, 16) + '\n' +
        padding + "\tAddress: 0x" + MathLib::ToString(address, 16) + '\n' +
        padding + "\tOffset: 0x" + MathLib::ToString(offset, 16) + '\n' +
        padding + "\tSize: " + MathLib::ToString(size, 10) + '\n' +
        padding + "\tLink: 0x" + MathLib::ToString(link, 16) + '\n' +
        padding + "\tInfo: 0x" + MathLib::ToString(info, 16) + '\n' +
        padding + "\tAlignment: " + MathLib::ToString(alignment, 10) + '\n' +
        padding + "\tEntry size: " + MathLib::ToString(entrySize, 10) + '\n' +
    padding + '}';
}