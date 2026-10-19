#include "X509.hpp"

namespace MathLib {
    DERTerm CreateX509CertificateTemplate(DERTerm& attributeTypeAndValue, DERTerm& relativeDistinguishedName) {
        attributeTypeAndValue = DERTerm("TypeAndValue", MakeArray<DERTerm>(
            DERTerm(DERTerm::Type::ObjectIdentifier, "Type"),
            DERTerm(DERTerm::Type::String, "Value")
        ));
        relativeDistinguishedName = DERTerm("RelativeDistinguishedName", &attributeTypeAndValue);
        return DERTerm("Root", MakeArray<DERTerm>(
            DERTerm("Certificate", MakeArray<DERTerm>(
                DERTerm(DERTerm::Type::Integer, "Version"),
                DERTerm(DERTerm::Type::Integer, "SerialNumber"),
                DERTerm("Signature", MakeArray<DERTerm>(
                    DERTerm(DERTerm::Type::ObjectIdentifier, "Algorithm"),
                    DERTerm(DERTerm::Type::Unknown, "Parameters")
                )),
                DERTerm("IssuerName", &relativeDistinguishedName),
                DERTerm("ValidityPeriod", MakeArray<DERTerm>(
                    DERTerm(DERTerm::Type::Date, "NotBefore"),
                    DERTerm(DERTerm::Type::Date, "NotAfter")
                )),
                DERTerm("SubjectName", &relativeDistinguishedName),
                DERTerm("SubjectPublicKeyInfo", MakeArray<DERTerm>(
                    DERTerm("Algorithm", MakeArray<DERTerm>(
                        DERTerm(DERTerm::Type::ObjectIdentifier, "Algorithm"),
                        DERTerm(DERTerm::Type::Unknown, "Parameters")
                    )),
                    DERTerm(DERTerm::Type::ByteArray, "PublicKey")
                ))
            )),
            DERTerm("SignatureAlgorithm", MakeArray<DERTerm>(
                DERTerm(DERTerm::Type::ObjectIdentifier, "Algorithm"),
                DERTerm(DERTerm::Type::Unknown, "Parameters")
            )),
            DERTerm(DERTerm::Type::ByteArray, "Signature")
        ));
    }
    bool IsX509CertificateValid(const DERTerm& certificate) {
        // TODO:
        (void)certificate;
        return true;
    }
    bool IsX509CertificateOwnedBy(const DERTerm& certificate, const MathLib::Sequence<char>& host) {
        // TODO:
        (void)certificate;
        (void)host;
        return true;
    }
    bool IsX509CertificateSignedBy(const DERTerm& certificate, const DERTerm& signerCertificate) {
        // TODO:
        (void)certificate;
        (void)signerCertificate;
        return true;
    }
}