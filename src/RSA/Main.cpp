#include <Cryptography/Cipher/RSAPKCS1v1v5.hpp>
#include <Interfaces/Sequence/ByteArray.hpp>
#include <FileSystem/FileSystem.hpp>
#include <DER/DER.hpp>
#include <iostream>

void Main(int argc, char** argv, MathLib::FileSystem& fs) {
    if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <RSA private key>");
    MathLib::CipherKey publicKey;
    MathLib::CipherKey privateKey;
    if (true) {
        MathLib::DERTerm keyWrapper;
        if (!MathLib::ParseDERFromPEM(keyWrapper, fs.Open(MathLib::String(argv[1]), MathLib::OpenMode::Read).ReadUntil('\0'), "PRIVATE KEY", 0)) MathLib::Panic("Failed to load key");
        if (keyWrapper.type != MathLib::DERTerm::Type::Sequence) MathLib::Panic("Failed to load key");
        const size_t size = keyWrapper.children.GetSize();
        if (size < 2) MathLib::Panic("Failed to load key");
        if (keyWrapper.children.AtUnsafe(size - 2).type != MathLib::DERTerm::Type::Sequence) MathLib::Panic("Failed to load key");
        if (keyWrapper.children.AtUnsafe(size - 2).children.IsEmpty()) MathLib::Panic("Failed to load key");
        if (keyWrapper.children.AtUnsafe(size - 2).children.At(0).value != MathLib::Array<uint8_t>((const uint8_t*)"1.2.840.113549.1.1.1", 20)) MathLib::Panic("Failed to load key");
        
        if (keyWrapper.children.AtUnsafe(size - 1).type != MathLib::DERTerm::Type::ByteArray) MathLib::Panic("Failed to load key");
        MathLib::DERTerm key;
        if (!MathLib::ParseDERFromSequence(key, keyWrapper.children.AtUnsafe(size - 1).value)) MathLib::Panic("Failed to load key");
        if (key.type != MathLib::DERTerm::Type::Sequence) MathLib::Panic("Failed to load key");
        if (key.children.GetSize() < 4) MathLib::Panic("Failed to load key");
        publicKey = MathLib::MakeArray<MathLib::CipherKey>(
            key.children.At(1).value,
            key.children.At(2).value
        );
        privateKey = MathLib::MakeArray<MathLib::CipherKey>(
            key.children.At(1).value,
            key.children.At(3).value
        );
    }
    // TODO: Test after implementing faster divison algorithm
    MathLib::RSAPKCS1v1v5 rsa;
    std::cout << "Public key: " << publicKey << std::endl;
    std::cout << "Private key: " << privateKey << std::endl;
    const MathLib::Array<uint8_t> message = MathLib::MakeArray<uint8_t>(0x00);
    std::cout << "Message: {\n" << MathLib::DumpMemory<uint8_t>(message, 16, false, "\t") << "\n}" << std::endl;
    const MathLib::Array<uint8_t> encrypted = rsa.Encrypt(message, publicKey);
    std::cout << "Encrypted: {\n" << MathLib::DumpMemory<uint8_t>(encrypted, 16, false, "\t") << "\n}" << std::endl;
    const MathLib::Array<uint8_t> decrypted = rsa.Decrypt(encrypted, privateKey);
    std::cout << "Decrypted: {\n" << MathLib::DumpMemory<uint8_t>(decrypted, 16, false, "\t") << "\n}" << std::endl;
}