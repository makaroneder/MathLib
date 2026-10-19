#include <FileSystem/FileSystem.hpp>
#include <DER/X509.hpp>
#include <DER/DER.hpp>
#include <Logger.hpp>

void Main(int argc, char** argv, MathLib::FileSystem& fs) {
    if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <ASN.1 binary file>");
    MathLib::DERTerm attributeTypeAndValue;
    MathLib::DERTerm relativeDistinguishedName;
    MathLib::DERTerm certificate = MathLib::CreateX509CertificateTemplate(attributeTypeAndValue, relativeDistinguishedName);
    if (true) {
        MathLib::File file = fs.Open(MathLib::String(argv[1]), MathLib::OpenMode::Read);
        if (!MathLib::ParseDER(certificate, file)) MathLib::Panic("Failed to parse DER");
    }
    LogString(certificate.ToString());
    LogChar('\n');
}