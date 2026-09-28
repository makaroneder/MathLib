#ifndef TLS_RecordHeader_H
#define TLS_RecordHeader_H
#include <stdint.h>

namespace MathLib {
    struct TLSRecordHeader {
        enum class Type : uint8_t {
            ChangeCipherSpec = 0x14,
            Alert,
            Handshake,
            ApplicationData,
        };
        Type type;
        uint16_t version;
        uint16_t size;

        TLSRecordHeader(void);
        TLSRecordHeader(Type type, uint16_t version, uint16_t size);
    } __attribute__((packed));
}

#endif