#ifndef MathLib_DER_Term_H
#define MathLib_DER_Term_H
#include "../Interfaces/Printable.hpp"

namespace MathLib {
    struct DERTerm;
    struct DERTerm : Comparable<DERTerm>, Printable {
        enum class Type : uint8_t {
            Unknown,
            Sequence,
            Integer,
            ByteArray,
            ObjectIdentifier,
            String,
            Bool,
            Date,
        };
        static constexpr const char* objectIdentifiers[] = {
            "1.2.840.113549.1.1.1", "RSA encryption",
            "1.2.840.113549.1.1.11", "SHA256 with RSA encryption",
            "1.3.6.1.4.1.11129.2.4.2", "Signed certificate timestamp list",
            "1.3.6.1.5.5.7.1.1", "Authority information access",
            "2.5.4.3", "Common name",
            "2.5.4.6", "Country name",
            "2.5.4.10", "Organization name",
            "2.5.29.14", "Subject key identifier",
            "2.5.29.15", "Key usage",
            "2.5.29.17", "Subject alternative name",
            "2.5.29.19", "Basic constraints",
            "2.5.29.31", "Certificate revocation list distribution points",
            "2.5.29.32", "Certificate policies",
            "2.5.29.35", "Authority key identifier",
            "2.5.29.37", "Extended key usage",
        };
        String name;
        Array<DERTerm> children;
        Array<uint8_t> value;
        const DERTerm* memberTemplate;
        Type type;

        DERTerm(void);
        DERTerm(Type type, const String& name);
        DERTerm(Type type, const String& name, const Array<uint8_t>& value);
        DERTerm(const String& name, const Array<DERTerm>& children);
        DERTerm(const String& name, const DERTerm* memberTemplate);
        [[nodiscard]] virtual bool Equals(const DERTerm& other) const override;
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
        [[nodiscard]] bool CheckTypeCompatibility(Type readType);
    };
}

#endif