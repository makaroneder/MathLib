#include "ELFSectionHeader.hpp"
#include <String.hpp>

MathLib::String ELFSectionHeader::ToString(const MathLib::String& padding) const {
    MathLib::String ret = padding + "{\n";
    ret += padding + "\tName: 0x" + MathLib::ToString(name, 16) + '\n';
    ret += padding + "\tType: 0x" + MathLib::ToString(type, 16) + '\n';
    ret += padding + "\tFlags: 0x" + MathLib::ToString(flags, 16) + '\n';
    ret += padding + "\tAddress: 0x" + MathLib::ToString(address, 16) + '\n';
    ret += padding + "\tOffset: 0x" + MathLib::ToString(offset, 16) + '\n';
    ret += padding + "\tSize: " + MathLib::ToString(size, 10) + '\n';
    ret += padding + "\tLink: 0x" + MathLib::ToString(link, 16) + '\n';
    ret += padding + "\tInfo: 0x" + MathLib::ToString(info, 16) + '\n';
    ret += padding + "\tAlignment: " + MathLib::ToString(alignment, 10) + '\n';
    ret += padding + "\tEntry size: " + MathLib::ToString(entrySize, 10) + '\n';
    return ret + padding + '}';
}