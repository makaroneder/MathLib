#ifndef TestResult_H
#define TestResult_H
#include <Typedefs.hpp>

struct TestResult;
struct TestResult : MathLib::Comparable<TestResult> {
    MathLib::Array<uint8_t> key;
    MathLib::Array<uint8_t> initializationVector;
    MathLib::Array<uint8_t> plainText;
    MathLib::Array<uint8_t> cipherText;

    TestResult(void);
    TestResult(const MathLib::Sequence<char>& str, size_t& i);

    protected:
    [[nodiscard]] virtual bool Equals(const TestResult& other) const override;
};

#endif