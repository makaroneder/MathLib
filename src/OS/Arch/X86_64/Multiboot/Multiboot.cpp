#ifdef __x86_64__
#include "Multiboot.hpp"
#include <String.hpp>
#include <Host.hpp>

MathLib::String MultibootAPM::ToString(const MathLib::Sequence<char>& padding) const {
    MathLib::String ret = MathLib::CollectionToString(padding) + "APM: {\n";
    ret += MathLib::CollectionToString(padding) + "\tVersion: 0x" + MathLib::ToString(version, 16) + '\n';
    ret += MathLib::CollectionToString(padding) + "\t32 bit code segment: 0x" + MathLib::ToString(codeSegment, 16) + '\n';
    ret += MathLib::CollectionToString(padding) + "\tOffset: 0x" + MathLib::ToString(offset, 16) + '\n';
    ret += MathLib::CollectionToString(padding) + "\t16 bit code segment: 0x" + MathLib::ToString(codeSegment16, 16) + '\n';
    ret += MathLib::CollectionToString(padding) + "\t32 bit data segment: 0x" + MathLib::ToString(dataSegment, 16) + '\n';
    ret += MathLib::CollectionToString(padding) + "\tFlags: 0x" + MathLib::ToString(flags, 16) + '\n';
    ret += MathLib::CollectionToString(padding) + "\t32 bit code segment length: 0x" + MathLib::ToString(codeSegmentLength, 16) + '\n';
    ret += MathLib::CollectionToString(padding) + "\t16 bit code segment length: 0x" + MathLib::ToString(codeSegment16Length, 16) + '\n';
    ret += MathLib::CollectionToString(padding) + "\t32 bit data segment length: 0x" + MathLib::ToString(dataSegmentLength, 16) + '\n';
    return ret + padding + "}\n";
}
MathLib::String MultibootFramebuffer::ToString(const MathLib::Sequence<char>& padding) const {
    MathLib::String ret = MathLib::CollectionToString(padding) + "Framebuffer: {\n";
    ret += MathLib::CollectionToString(padding) + "\tAddress: 0x" + MathLib::ToString(address, 16) + '\n';
    ret += MathLib::CollectionToString(padding) + "\tWidth: " + MathLib::ToString(width) + '\n';
    ret += MathLib::CollectionToString(padding) + "\tHeight: " + MathLib::ToString(height) + '\n';
    ret += MathLib::CollectionToString(padding) + "\tDepth: " + MathLib::ToString(bitsPerPixel) + '\n';
    ret += MathLib::CollectionToString(padding) + "\tPitch: " + MathLib::ToString(pitch) + '\n';
    ret += MathLib::CollectionToString(padding) + "\tType: " + (type < Type::TypeCount ? typeStr[(uint8_t)type] : "Unknown (0x"_M + MathLib::ToString((uint8_t)type, 16) + ')') + '\n';
    return ret + padding + "}\n";
}

#endif