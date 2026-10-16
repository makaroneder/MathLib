#include <FileSystem/FileSystem.hpp>
#include <DER/DER.hpp>
#include <iostream>

void Main(int argc, char** argv, MathLib::FileSystem& fs) {
    if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <ASN.1 binary file>");
    const MathLib::DERTerm attributeTypeAndValue = MathLib::DERTerm("TypeAndValue", MathLib::MakeArray<MathLib::DERTerm>(
        MathLib::DERTerm(MathLib::DERTerm::Type::ObjectIdentifier, "Type"),
        MathLib::DERTerm(MathLib::DERTerm::Type::String, "Value")
    ));
    const MathLib::DERTerm relativeDistinguishedName = MathLib::DERTerm("RelativeDistinguishedName", &attributeTypeAndValue);
    MathLib::DERTerm certificate = MathLib::DERTerm("Root", MathLib::MakeArray<MathLib::DERTerm>(
        MathLib::DERTerm("Certificate", MathLib::MakeArray<MathLib::DERTerm>(
            MathLib::DERTerm(MathLib::DERTerm::Type::Integer, "Version"), // ?
            MathLib::DERTerm(MathLib::DERTerm::Type::Integer, "SerialNumber"),
            MathLib::DERTerm("Signature", MathLib::MakeArray<MathLib::DERTerm>(
                MathLib::DERTerm(MathLib::DERTerm::Type::ObjectIdentifier, "Algorithm"),
                MathLib::DERTerm(MathLib::DERTerm::Type::Unknown, "Parameters")
            )),
            MathLib::DERTerm("IssuerName", &relativeDistinguishedName),
            MathLib::DERTerm("ValidityPeriod", MathLib::MakeArray<MathLib::DERTerm>(
                MathLib::DERTerm(MathLib::DERTerm::Type::Date, "NotBefore"),
                MathLib::DERTerm(MathLib::DERTerm::Type::Date, "NotAfter")
            )),
            MathLib::DERTerm("SubjectName", &relativeDistinguishedName),
            MathLib::DERTerm("SubjectPublicKeyInfo", MathLib::MakeArray<MathLib::DERTerm>(
                MathLib::DERTerm("Algorithm", MathLib::MakeArray<MathLib::DERTerm>(
                    MathLib::DERTerm(MathLib::DERTerm::Type::ObjectIdentifier, "Algorithm"),
                    MathLib::DERTerm(MathLib::DERTerm::Type::Unknown, "Parameters")
                )),
                MathLib::DERTerm(MathLib::DERTerm::Type::ByteArray, "PublicKey")
            ))
        )),
        MathLib::DERTerm("SignatureAlgorithm", MathLib::MakeArray<MathLib::DERTerm>(
            MathLib::DERTerm(MathLib::DERTerm::Type::ObjectIdentifier, "Algorithm"),
            MathLib::DERTerm(MathLib::DERTerm::Type::Unknown, "Parameters")
        )),
        MathLib::DERTerm(MathLib::DERTerm::Type::ByteArray, "Signature")
    ));
    if (!MathLib::ParseDER(certificate, fs.Open(MathLib::String(argv[1]), MathLib::OpenMode::Read).ReadAll())) MathLib::Panic("Failed to parse DER");
    std::cout << certificate << std::endl;
}