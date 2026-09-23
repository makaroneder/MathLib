#ifndef TLSAlert_H
#define TLSAlert_H
#include <stdint.h>

struct TLSAlert {
    enum class Level : uint8_t {
        Warning = 0x01,
        Fatal,
    };
    enum class Description : uint8_t {
        CloseNotify = 0x00,
        UnexpectedMessage = 0x0a,
        BadRecordMAC = 0x14,
        DecryptionFailed,
        RecordOverflow,
        DecompressionFailure = 0x1e,
        HandshakeFailure = 0x28,
        NoCertificate,
        BadCertificate,
        UnsupportedCertificate,
        CertificateRevoked,
        CertificateExpired,
        CertificateUnknown,
        IllegalParameter,
        UnknownCA,
        AccessDenied,
        DecodeError,
        DecryptError,
        ExportRestriction = 0x3c,
        ProtocolVersion = 0x46,
        InsufficientSecurity,
        InternalError = 0x50,
        UserCanceled = 0x5a,
        NoRenegotiation = 0x64,
        UnsupportedExtension = 0x6e,
    };

    Level level;
    Description description;
} __attribute__((packed));

#endif