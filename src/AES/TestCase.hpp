#ifndef TestCase_H
#define TestCase_H
#include "TestResult.hpp"
#include <Cryptography/Cipher/AES.hpp>

struct TestCase : MathLib::Printable {
    enum class ModeOfOperation : uint8_t {
        CBC,
        ECB,
        OFB,
    };

    TestCase(const MathLib::Sequence<char>& str);
    [[nodiscard]] bool IsValid(void) const;
    [[nodiscard]] size_t GetTests(void) const;
    [[nodiscard]] MathLib::CipherKey MakeKey(size_t i) const;
    [[nodiscard]] bool IsOkay(size_t i) const;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;

    private:
    MathLib::String name;
    MathLib::Array<TestResult> results;
    MathLib::AES::Rounds rounds;
    ModeOfOperation modeOfOperation;
    bool valid;
};

#endif