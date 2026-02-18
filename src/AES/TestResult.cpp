#include "TestResult.hpp"
#include "Value.hpp"

[[nodiscard]] MathLib::Array<uint8_t> StringToBytes(const MathLib::Sequence<char>& str) {
    const size_t size = str.GetSize();
    const bool start = size % 2;
    const size_t halfSize = size / 2;
    MathLib::Array<uint8_t> ret = halfSize + start;
    if (start) ret.AtUnsafe(0) = MathLib::StringToU4(str.AtUnsafe(0));
    for (size_t i = 0; i < halfSize; i++)
        ret.AtUnsafe(i + start) = MathLib::StringToU8(str.AtUnsafe(2 * i + start), str.AtUnsafe(2 * i + start + 1));
    return ret;
}
TestResult::TestResult(void) {}
TestResult::TestResult(const MathLib::Sequence<char>& str, size_t& i) {
    const Value count = ReadValue(str, i);
    if (count.name != "COUNT"_M) return;
    const Value keyStr = ReadValue(str, i);
    if (keyStr.name != "KEY"_M) return;
    const Value initializationVectorStr = ReadValue(str, i);
    if (initializationVectorStr.name != "IV"_M) return;
    const Value plainTextStr = ReadValue(str, i);
    if (plainTextStr.name != "PLAINTEXT"_M) return;
    const Value cipherTextStr = ReadValue(str, i);
    if (cipherTextStr.name != "CIPHERTEXT"_M) return;
    key = StringToBytes(keyStr.value);
    initializationVector = StringToBytes(initializationVectorStr.value);
    plainText = StringToBytes(plainTextStr.value);
    cipherText = StringToBytes(cipherTextStr.value);
}
bool TestResult::Equals(const TestResult& other) const {
    return key == other.key && initializationVector == other.initializationVector && plainText == other.plainText && cipherText == other.cipherText;
}