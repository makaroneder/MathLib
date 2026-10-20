#include "X509.hpp"
#include "DER.hpp"

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
                DERTerm("Issuer", &relativeDistinguishedName),
                DERTerm("ValidityPeriod", MakeArray<DERTerm>(
                    DERTerm(DERTerm::Type::Date, "NotBefore"),
                    DERTerm(DERTerm::Type::Date, "NotAfter")
                )),
                DERTerm("Subject", &relativeDistinguishedName),
                DERTerm("SubjectPublicKey", MakeArray<DERTerm>(
                    DERTerm("PublicKeyAlgorithm", MakeArray<DERTerm>(
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
    bool ParseX509Certificate(DERTerm& certificate, Readable& certificateData) {
        DERTerm attributeTypeAndValue;
        DERTerm relativeDistinguishedName;
        certificate = CreateX509CertificateTemplate(attributeTypeAndValue, relativeDistinguishedName);
        if (!ParseDER(certificate, certificateData)) return false;
        certificate.children.AtUnsafe(0).children.At(certificate.children.AtUnsafe(0).children.GetSize() - 1).type = DERTerm::Type::Sequence;
        certificate.children.AtUnsafe(0).children.At(certificate.children.AtUnsafe(0).children.GetSize() - 1).name = "Extensions";
        for (DERTerm& child : certificate.children.AtUnsafe(0).children.At(certificate.children.AtUnsafe(0).children.GetSize() - 1).children) {
            const size_t start = 7;
            const DERTerm save = child;
            child = DERTerm(DERTerm::Type::Sequence, SubString(child.name, start, child.name.GetSize() - start));
            if (!child.children.Add(save.children.At(0))) return false;
            child.children.At(0).name = "Type";
            const size_t size = save.children.GetSize();
            if (size != 2) {
                if (!child.children.Add(save.children.At(1))) return false;
                child.children.At(1).name = "Critical";
            }
            else if (!child.children.Add(DERTerm(DERTerm::Type::Bool, "Critical", MakeArray<uint8_t>(false)))) return false;
            if (!child.children.Add(save.children.At(size - 1))) return false;
            child.children.At(2).name = "Value";
        }
        return true;
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