#include "TestCase.hpp"
#include <Cryptography/ModeOfOperation/CBC.hpp>
#include <Cryptography/ModeOfOperation/ECB.hpp>
#include <Cryptography/ModeOfOperation/OFB.hpp>

TestCase::TestCase(const MathLib::Sequence<char>& str) {
    valid = false;
    size_t i = 0;
    MathLib::SkipWhiteSpace(str, i);
    if (!str.Contains("# CAVS 11.1"_M, i)) return;
    i += 11;
    
    MathLib::SkipWhiteSpace(str, i);
    if (!str.Contains("# Config info for aes_values"_M, i)) return;
    i += 28;
    
    MathLib::SkipWhiteSpace(str, i);
    if (!str.Contains("# AESVS "_M)) return;
    i += 8;
    while (!MathLib::IsWhiteSpace(str.AtUnsafe(i))) name += str.AtUnsafe(i++);
    if (!str.Contains(" test data for "_M, i)) return;
    i += 15;

    MathLib::String mode;
    while (!MathLib::IsWhiteSpace(str.AtUnsafe(i))) mode += str.AtUnsafe(i++);
    MathLib::SkipWhiteSpace(str, i);
    if (mode == "CBC") modeOfOperation = ModeOfOperation::CBC;
    else if (mode == "ECB") modeOfOperation = ModeOfOperation::ECB;
    else if (mode == "OFB") modeOfOperation = ModeOfOperation::OFB;
    else return;

    MathLib::SkipWhiteSpace(str, i);
    if (!str.Contains("# State : Encrypt and Decrypt"_M, i)) return;
    i += 29;

    MathLib::SkipWhiteSpace(str, i);
    if (!str.Contains("# Key Length : "_M, i)) return;
    i += 15;

    MathLib::String bits;
    while (!MathLib::IsWhiteSpace(str.AtUnsafe(i))) bits += str.AtUnsafe(i++);
    MathLib::SkipWhiteSpace(str, i);
    if (bits == "128") rounds = MathLib::AES::Rounds::AES128;
    else if (bits == "192") rounds = MathLib::AES::Rounds::AES192;
    else if (bits == "256") rounds = MathLib::AES::Rounds::AES256;
    else return;

    while (str.AtUnsafe(i) != '\n') i++;
    MathLib::SkipWhiteSpace(str, i);
    if (!str.Contains("[ENCRYPT]"_M, i)) return;
    i += 9;

    while (true) {
        MathLib::SkipWhiteSpace(str, i);
        if (i >= str.GetSize() || str.AtUnsafe(i) == '[') break;
        if (!results.Add(TestResult(str, i))) return;
    }
    valid = true;
}
bool TestCase::IsValid(void) const {
    return valid;
}
size_t TestCase::GetTests(void) const {
    return results.GetSize();
}
MathLib::CipherKey TestCase::MakeKey(size_t i) const {
    return valid && i < results.GetSize() ? MathLib::CipherKey(MathLib::MakeArray<MathLib::CipherKey>(
        MathLib::CipherKey(results.AtUnsafe(i).initializationVector),
        MathLib::AES().GenerateKey(MathLib::CipherKey(MathLib::MakeArray<MathLib::CipherKey>(
            MathLib::CipherKey(MathLib::ByteArray::ToByteArray<MathLib::AES::Rounds>(MathLib::MakeArray<MathLib::AES::Rounds>(rounds))),
            MathLib::CipherKey(results.AtUnsafe(i).key)
        )))
    )) : MathLib::CipherKey();
}
bool TestCase::IsOkay(size_t i) const {
    if (!valid) return false;
    MathLib::AES aes;
    MathLib::Cipher* cipher = nullptr;
    switch (modeOfOperation) {
        case ModeOfOperation::CBC: {
            cipher = new MathLib::CBC(aes);
            break;
        }
        case ModeOfOperation::ECB: {
            cipher = new MathLib::ECB(aes);
            break;
        }
        case ModeOfOperation::OFB: {
            cipher = new MathLib::OFB(aes);
            break;
        }
        default: return false;
    }
    if (!cipher) return false;
    const MathLib::CipherKey key = MakeKey(i);
    if (results.AtUnsafe(i).plainText != cipher->Decrypt(results.AtUnsafe(i).cipherText, key)) return false;
    if (results.AtUnsafe(i).cipherText != cipher->Encrypt(results.AtUnsafe(i).plainText, key)) return false;
    delete cipher;
    return true;
}
MathLib::String TestCase::ToString(const MathLib::Sequence<char>& padding) const {
    if (!valid) return "";
    MathLib::String ret = MathLib::CollectionToString(padding);
    switch (modeOfOperation) {
        case ModeOfOperation::CBC: {
            ret += "CBC";
            break;
        }
        case ModeOfOperation::ECB: {
            ret += "ECB";
            break;
        }
        case ModeOfOperation::OFB: {
            ret += "OFB";
            break;
        }
        default: return "";
    }
    ret += name;
    switch (rounds) {
        case MathLib::AES::Rounds::AES128: {
            ret += "128";
            break;
        }
        case MathLib::AES::Rounds::AES192: {
            ret += "192";
            break;
        }
        case MathLib::AES::Rounds::AES256: {
            ret += "256";
            break;
        }
        default: return "";
    }
    return ret += ".rsp";
}