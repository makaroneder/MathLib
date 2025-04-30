#include "GZIPHeader.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    GZIPHeader::GZIPHeader(void) : signature(0), compressionMethod(), text(false), crc16(false), extraData(false), name(false), comment(false), reserved(0), time(0), extraFlags(), os(OperatingSystem::Unknown) {
        EmptyBenchmark
    }
    GZIPHeader::GZIPHeader(bool text, bool crc16, bool extraData, bool name, bool comment, ExtraFlags extraFlags, OperatingSystem os) : signature(expectedSignature), compressionMethod(CompressionMethod::Deflate), text(text), crc16(crc16), extraData(extraData), name(name), comment(comment), reserved(0), time(0), extraFlags(extraFlags), os(os) {
        EmptyBenchmark
    }
    bool GZIPHeader::IsValid(void) const {
        StartAndReturnFromBenchmark(signature == expectedSignature);
    }
}