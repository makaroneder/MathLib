#ifndef MathLib_Cryptography_Compressor_GZIPHeader_H
#define MathLib_Cryptography_Compressor_GZIPHeader_H
#include <stdint.h>

namespace MathLib {
    struct GZIPHeader {
        enum class CompressionMethod : uint8_t {
            Deflate = 8,
        };
        enum class ExtraFlags : uint8_t {
            MaximumCompression = 2,
            FastestCompression = 4,
        };
        enum class OperatingSystem : uint8_t {
            FAT = 0x00,
            Amiga,
            VMS,
            Unix,
            VM,
            AtariTOS,
            HPFS,
            Macintosh,
            ZSystem,
            CPM,
            TOPS20,
            NTFS,
            QDOS,
            AcornRISCOS,
            Unknown = UINT8_MAX,
        };
        static constexpr uint16_t expectedSignature = 0x8b1f;
        uint16_t signature;
        CompressionMethod compressionMethod;
        bool text : 1;
        bool crc16 : 1;
        bool extraData : 1;
        bool name : 1;
        bool comment : 1;
        uint8_t reserved : 3;
        uint32_t time;
        ExtraFlags extraFlags;
        OperatingSystem os;

        GZIPHeader(void);
        GZIPHeader(bool text, bool crc16, bool extraData, bool name, bool comment, ExtraFlags extraFlags, OperatingSystem os);
        [[nodiscard]] bool IsValid(void) const;
    } __attribute__((packed));
}

#endif