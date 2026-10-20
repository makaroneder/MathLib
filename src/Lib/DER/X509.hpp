#ifndef MathLib_DER_X509_H
#define MathLib_DER_X509_H
#include "DERTerm.hpp"
#include "../Interfaces/Readable.hpp"

namespace MathLib {
    [[nodiscard]] DERTerm CreateX509CertificateTemplate(DERTerm& attributeTypeAndValue, DERTerm& relativeDistinguishedName);
    [[nodiscard]] bool ParseX509Certificate(DERTerm& certificate, Readable& certificateData);
    [[nodiscard]] bool IsX509CertificateValid(const DERTerm& certificate);
    [[nodiscard]] bool IsX509CertificateOwnedBy(const DERTerm& certificate, const MathLib::Sequence<char>& host);
    [[nodiscard]] bool IsX509CertificateSignedBy(const DERTerm& certificate, const DERTerm& signerCertificate);
}

#endif