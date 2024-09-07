#include "Multiboot.hpp"
#include <String.hpp>
#include <Host.hpp>

String MultibootAPM::ToString(const String& padding) const {
    String ret = "APM: {\n";
    ret += padding + "\tVersion: 0x" + ::ToString(version, 16) + '\n';
    ret += padding + "\t32 bit code segment: 0x" + ::ToString(codeSegment, 16) + '\n';
    ret += padding + "\tOffset: 0x" + ::ToString(offset, 16) + '\n';
    ret += padding + "\t16 bit code segment: 0x" + ::ToString(codeSegment16, 16) + '\n';
    ret += padding + "\t32 bit data segment: 0x" + ::ToString(dataSegment, 16) + '\n';
    ret += padding + "\tFlags: 0x" + ::ToString(flags, 16) + '\n';
    ret += padding + "\t32 bit code segment length: 0x" + ::ToString(codeSegmentLength, 16) + '\n';
    ret += padding + "\t16 bit code segment length: 0x" + ::ToString(codeSegment16Length, 16) + '\n';
    ret += padding + "\t32 bit data segment length: 0x" + ::ToString(dataSegmentLength, 16) + '\n';
    return ret + padding + "}\n";
}
String MultibootFramebuffer::ToString(const String& padding) const {
    String ret = padding + "Framebuffer: {\n";
    ret += padding + "\tAddress: 0x" + ::ToString(address, 16) + '\n';
    ret += padding + "\tWidth: " + ::ToString(width) + '\n';
    ret += padding + "\tHeight: " + ::ToString(height) + '\n';
    ret += padding + "\tDepth: " + ::ToString(bitsPerPixel) + '\n';
    ret += padding + "\tPitch: " + ::ToString(pitch) + '\n';
    ret += padding + "\tType: " + (type < Type::TypeCount ? typeStr[(uint8_t)type] : String("Unknown(0x") + ::ToString((uint8_t)type, 16) + ')') + '\n';
    return ret + padding + "}\n";
}