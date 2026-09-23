#include "Test.hpp"
#include "../Math/Matrix.hpp"
#include "../Math/Sigmoid.hpp"
#include "../Physics/SIUnits.hpp"
#include "../Math/Trigonometry.hpp"
#include "../FileSystem/Cipher/Ar/Ar.hpp"
#include "../Cryptography/Cipher/PKCS7.hpp"
#include "../Cryptography/Cipher/ROT13.hpp"
#include "../Cryptography/Code/UnaryCode.hpp"
#include "../Cryptography/Compressor/ZLib.hpp"
#include "../Cryptography/Compressor/GZIP.hpp"
#include "../Interfaces/ComparisionFunction.hpp"
#include "../Cryptography/Cipher/NullCipher.hpp"
#include "../Cryptography/Compressor/Deflate.hpp"
#include "../Cryptography/Code/DeltaEncoding.hpp"
#include "../Cryptography/Compressor/PackBits.hpp"
#include "../Cryptography/Code/RepetitionCode.hpp"
#include "../Cryptography/Cipher/AtbashCipher.hpp"
#include "../Cryptography/OneWayCipher/SHA/SHA1.hpp"
#include "../Cryptography/Cipher/IdentityCipher.hpp"
#include "../Cryptography/Code/RunLengthEncoding.hpp"
#include "../FileSystem/Cipher/TAR/GNUTAR/GNUTAR.hpp"
#include "../Cryptography/OneWayCipher/SHA/SHA256.hpp"
#include "../Interfaces/Sequence/IdentitySequence.hpp"
#include "../Cryptography/OneWayCipher/SHA/SHAPadding.hpp"
#include "../Cryptography/Cipher/PolyalphabeticCipher.hpp"
#include "../FileSystem/Cipher/TAR/UnixStandardTAR/UnixStandardTAR.hpp"
#include "../Cryptography/Compressor/BytePairEncoding/BytePairEncoding.hpp"

// TODO: Add test cases

namespace MathLib {
    bool Test::ReportRecord(const Record& record) {
        return records.Add(record);
    }
    bool Test::Passed(void) const {
        size_t ret = 0;
        for (const Record& record : records) ret += record.passed;
        return ret == records.GetSize();
    }
    String Test::ToString(const Sequence<char>& padding) const {
        String ret;
        size_t passed = 0;
        for (const Record& record : records) {
            ret += record.ToString(padding) + '\n';
            passed += record.passed;
        }
        return ret + padding + Formatter<size_t>::ToString(passed) + '/' + Formatter<size_t>::ToString(records.GetSize()) + " tests passed";
    }
    void TestSort(Test& test, const Sequence<uint8_t>& data, const Function<bool, Array<uint8_t>&, const Function<bool, uint8_t, uint8_t>&>& sort, ComparisionFunctionType type) {
        Array<uint8_t> sorted = CollectionToArray<uint8_t>(data);
        const ComparisionFunction<uint8_t> compare = ComparisionFunction<uint8_t>(type);
        TestOperation(test, sort(sorted, compare));
        TestOperation(test, sorted.IsSorted(compare));
    }
    Test TestSelf(void) {
        Test test;
        TestOperation(test, IsNaN(nan));
        TestOperation(test, IsInf(infinity));
        TestOperation(test, IsInf(-infinity));
        TestOperation(test, IsNaN(complex_t(nan, 0)));
        TestOperation(test, IsInf(complex_t(infinity, 0)));
        TestOperation(test, IsInf(complex_t(-infinity, 0)));
        TestOperation(test, IsNaN(complex_t(0, nan)));
        TestOperation(test, IsInf(complex_t(0, infinity)));
        TestOperation(test, IsInf(complex_t(0, -infinity)));
        TestOperation(test, IsNaN(complex_t(nan, nan)));
        TestOperation(test, IsInf(complex_t(infinity, infinity)));
        TestOperation(test, FloatsEqual<num_t>(1, 1));
        TestOperation(test, FloatsEqual<num_t>(StringToNumber("2.71"_M), 2.71));
        TestOperation(test, ToString(2.71).Contains(String("2.71")));
        TestOperation(test, ToUpper('a') == 'A');
        TestOperation(test, ToUpper('A') == 'A');
        TestOperation(test, ToUpper('1') == '1');
        TestOperation(test, ToUpper('#') == '#');
        TestOperation(test, !IsUpper('a'));
        TestOperation(test, IsUpper('A'));
        TestOperation(test, !IsUpper('1'));
        TestOperation(test, !IsUpper('#'));
        TestOperation(test, IsLower('a'));
        TestOperation(test, !IsLower('A'));
        TestOperation(test, !IsLower('1'));
        TestOperation(test, !IsLower('#'));
        TestOperation(test, IsAlpha('a'));
        TestOperation(test, IsAlpha('A'));
        TestOperation(test, !IsAlpha('1'));
        TestOperation(test, !IsAlpha('#'));
        TestOperation(test, !IsDigit('a'));
        TestOperation(test, !IsDigit('A'));
        TestOperation(test, IsDigit('1'));
        TestOperation(test, !IsDigit('#'));
        TestOperation(test, IsHexDigit('a'));
        TestOperation(test, IsHexDigit('A'));
        TestOperation(test, IsHexDigit('1'));
        TestOperation(test, !IsHexDigit('#'));
        TestOperation(test, IsAlphaDigit('a'));
        TestOperation(test, IsAlphaDigit('A'));
        TestOperation(test, IsAlphaDigit('1'));
        TestOperation(test, !IsAlphaDigit('#'));
        TestOperation(test, IsWhiteSpace(' '));
        TestOperation(test, IsWhiteSpace('\t'));
        TestOperation(test, IsWhiteSpace('\n'));
        TestOperation(test, IsWhiteSpace('\r'));
        TestOperation(test, !IsWhiteSpace('a'));
        TestOperation(test, !IsWhiteSpace('A'));
        TestOperation(test, !IsWhiteSpace('1'));
        TestOperation(test, !IsWhiteSpace('#'));
        TestOperation(test, BitwiseNot(0) == (size_t)~0);
        TestOperation(test, BitwiseNot(1) == (size_t)~1);
        TestOperation(test, BitwiseAnd(0, 0) == (size_t)(0 & 0));
        TestOperation(test, BitwiseAnd(1, 0) == (size_t)(1 & 0));
        TestOperation(test, BitwiseAnd(0, 1) == (size_t)(0 & 1));
        TestOperation(test, BitwiseAnd(1, 1) == (size_t)(1 & 1));
        TestOperation(test, BitwiseOr(0, 0) == (size_t)(0 | 0));
        TestOperation(test, BitwiseOr(1, 0) == (size_t)(1 | 0));
        TestOperation(test, BitwiseOr(0, 1) == (size_t)(0 | 1));
        TestOperation(test, BitwiseOr(1, 1) == (size_t)(1 | 1));
        TestOperation(test, BitwiseXor(0, 0) == (size_t)(0 ^ 0));
        TestOperation(test, BitwiseXor(1, 0) == (size_t)(1 ^ 0));
        TestOperation(test, BitwiseXor(0, 1) == (size_t)(0 ^ 1));
        TestOperation(test, BitwiseXor(1, 1) == (size_t)(1 ^ 1));
        TestOperation(test, BitwiseNand(0, 0) == (size_t)~(0 & 0));
        TestOperation(test, BitwiseNand(1, 0) == (size_t)~(1 & 0));
        TestOperation(test, BitwiseNand(0, 1) == (size_t)~(0 & 1));
        TestOperation(test, BitwiseNand(1, 1) == (size_t)~(1 & 1));
        TestOperation(test, BitwiseNor(0, 0) == (size_t)~(0 | 0));
        TestOperation(test, BitwiseNor(1, 0) == (size_t)~(1 | 0));
        TestOperation(test, BitwiseNor(0, 1) == (size_t)~(0 | 1));
        TestOperation(test, BitwiseNor(1, 1) == (size_t)~(1 | 1));
        TestOperation(test, BitwiseXnor(0, 0) == (size_t)~(0 ^ 0));
        TestOperation(test, BitwiseXnor(1, 0) == (size_t)~(1 ^ 0));
        TestOperation(test, BitwiseXnor(0, 1) == (size_t)~(0 ^ 1));
        TestOperation(test, BitwiseXnor(1, 1) == (size_t)~(1 ^ 1));
        TestOperation(test, RandomFloat() >= 0);
        TestOperation(test, RandomFloat() <= 1);
        TestOperation(test, FloatsEqual<num_t>(Abs(1), 1));
        TestOperation(test, FloatsEqual<num_t>(Abs(-1), 1));
        TestOperation(test, FloatsEqual<num_t>(Abs(complex_t(0, -1)), 1));
        TestOperation(test, FloatsEqual<num_t>(Abs(complex_t(0, 1)), 1));
        TestOperation(test, FloatsEqual<num_t>(Abs(complex_t(-1, 0)), 1));
        TestOperation(test, FloatsEqual<num_t>(Abs(complex_t(1, 0)), 1));
        TestOperation(test, FloatsEqual<num_t>(Max<num_t>(1, 0), 1));
        TestOperation(test, FloatsEqual<num_t>(Max<num_t>(0, 1), 1));
        TestOperation(test, FloatsEqual<num_t>(Min<num_t>(1, 0), 0));
        TestOperation(test, FloatsEqual<num_t>(Min<num_t>(0, 1), 0));
        TestOperation(test, FloatsEqual<num_t>(Pow(0, 0), 1));
        TestOperation(test, FloatsEqual<num_t>(Pow(1, 0), 1));
        TestOperation(test, FloatsEqual<num_t>(Pow(0, 1), 0));
        TestOperation(test, FloatsEqual<num_t>(Pow(1, 1), 1));
        TestOperation(test, Pow(complex_t(0, 0), complex_t(0, 0)) == complex_t(1, 0));
        TestOperation(test, Pow(complex_t(1, 0), complex_t(0, 0)) == complex_t(1, 0));
        TestOperation(test, Pow(complex_t(0, 0), complex_t(1, 0)) == complex_t(0, 0));
        TestOperation(test, Pow(complex_t(1, 0), complex_t(1, 0)) == complex_t(1, 0));
        TestOperation(test, Pow(complex_t(0, 1), complex_t(0, 0)) == complex_t(1, 0));
        TestOperation(test, Pow(complex_t(0, 1), complex_t(1, 0)) == complex_t(0, 1));
        TestOperation(test, FloatsEqual<num_t>(Sqrt(0), 0));
        TestOperation(test, FloatsEqual<num_t>(Sqrt(1), 1));
        TestOperation(test, Sqrt(complex_t(0, 0)) == complex_t(0, 0));
        TestOperation(test, Sqrt(complex_t(1, 0)) == complex_t(1, 0));
        TestOperation(test, Sqrt(complex_t(-1, 0)) == complex_t(0, 1));
        TestOperation(test, FloatsEqual<num_t>(Cbrt(1), 1));
        TestOperation(test, FloatsEqual<num_t>(NaturalLog(1), 0));
        TestOperation(test, FloatsEqual<num_t>(NaturalLog(e), 1));
        TestOperation(test, NaturalLog(complex_t(1, 0)) == complex_t(0, 0));
        TestOperation(test, NaturalLog(complex_t(e, 0)) == complex_t(1, 0));
        TestOperation(test, NaturalLog(complex_t(-1, 0)) == complex_t(0, pi));
        TestOperation(test, FloatsEqual<num_t>(Exp(0), 1));
        TestOperation(test, FloatsEqual<num_t>(Exp(1), e));
        TestOperation(test, Exp(complex_t(0, 0)) == complex_t(1, 0));
        TestOperation(test, Exp(complex_t(1, 0)) == complex_t(e, 0));
        TestOperation(test, Exp(complex_t(0, pi)) == complex_t(-1, 0));
        TestOperation(test, FloatsEqual<num_t>(Round(3.5), 4));
        TestOperation(test, FloatsEqual<num_t>(Round(3.4), 3));
        TestOperation(test, FloatsEqual<num_t>(Ceil(3.5), 4));
        TestOperation(test, FloatsEqual<num_t>(Ceil(3.4), 4));
        TestOperation(test, FloatsEqual<num_t>(Floor(3.5), 3));
        TestOperation(test, FloatsEqual<num_t>(Floor(3.4), 3));
        TestOperation(test, FloatsEqual<num_t>(Sin(0), 0));
        TestOperation(test, FloatsEqual<num_t>(Sin(pi), 0));
        TestOperation(test, Sin(complex_t(0, 0)) == complex_t(0, 0));
        TestOperation(test, Sin(complex_t(pi, 0)) == complex_t(0, 0));
        TestOperation(test, FloatsEqual<num_t>(InversedSin(0), 0));
        TestOperation(test, FloatsEqual<num_t>(InversedSin(1), pi / 2));
        TestOperation(test, FloatsEqual<num_t>(InversedSin(-1), -pi / 2));
        TestOperation(test, InversedSin(complex_t(0, 0)) == complex_t(0, 0));
        TestOperation(test, InversedSin(complex_t(1, 0)) == complex_t(pi / 2, 0));
        TestOperation(test, InversedSin(complex_t(-1, 0)) == complex_t(-pi / 2, 0));
        TestOperation(test, FloatsEqual<num_t>(InversedTan2(0, 0), 0));
        TestOperation(test, FloatsEqual<num_t>(InversedTan2(1, 0), pi / 2));
        TestOperation(test, FloatsEqual<num_t>(InversedTan2(0, 1), 0));
        TestOperation(test, FloatsEqual<num_t>(InversedTan2(1, 1), pi / 4));
        TestOperation(test, FloatsEqual<num_t>(Cos<num_t>(0), 1));
        TestOperation(test, FloatsEqual<num_t>(Cos<num_t>(2 * pi), 1));
        TestOperation(test, FloatsEqual<num_t>(InversedCos<num_t>(0), pi / 2));
        TestOperation(test, FloatsEqual<num_t>(InversedCos<num_t>(1), 0));
        TestOperation(test, FloatsEqual<num_t>(InversedCos<num_t>(-1), pi));
        TestOperation(test, FloatsEqual<num_t>(Sigmoid<num_t>(0), 0.5));
        TestOperation(test, FloatsEqual<num_t>(Factorial<num_t>(0, 1), 1));
        TestOperation(test, FloatsEqual<num_t>(Factorial<num_t>(1, 1), 1));
        TestOperation(test, FloatsEqual<num_t>(Sign<num_t>(1), 1));
        TestOperation(test, FloatsEqual<num_t>(Sign<num_t>(-1), -1));
        TestOperation(test, FloatsEqual<num_t>(Sign<num_t>(0), 0));
        TestOperation(test, MakeArray<size_t>(SIZE_MAX).At(0) == SIZE_MAX);
        TestOperation(test, BitReverse<uint8_t>(0b11111100) == 0b00111111);
        TestOperation(test, IsBetween(1, 0, 2));
        TestOperation(test, !IsBetween(3, 0, 2));
        TestOperation(test, !Expected<size_t>().HasValue());
        TestOperation(test, Expected<size_t>(1).HasValue());
        TestOperation(test, Interval<size_t>(1, 3).GetMin() == 1);
        TestOperation(test, Interval<size_t>(1, 3).GetMax() == 3);
        TestOperation(test, Interval<size_t>(1, 3).GetSize() == 2);
        TestOperation(test, Interval<size_t>(1, 3).Contains(2));
        TestOperation(test, !Interval<size_t>(1, 3).Contains(4));
        TestOperation(test, Interval<size_t>(1, 3).Clamp(2) == 2);
        TestOperation(test, Interval<size_t>(1, 3).Clamp(4) == 3);
        TestOperation(test, Kilometre<num_t>(1) == Metre<num_t>(1000));
        TestOperation(test, (Second<num_t>(1) * Second<num_t>(1)) == Second<num_t>(1).Pow(2));
        TestOperation(test, FloatsEqual<num_t>(matrix_t(1, 1, MakeArray<num_t>(2)).Normalize().GetLength(), 1));
        TestOperation(test, !matrix_t(1, 1, MakeArray<num_t>(0)).Normalize().GetLength());
        TestOperation(test, !matrix_t(1, 1, MakeArray<num_t>(0)).GetInverse().HasValue());
        TestOperation(test, matrix_t(1, 1, MakeArray<num_t>(1)).GetInverse().HasValue());
        const IdentitySequence<uint8_t> identitySequence = IdentitySequence<uint8_t>(UINT8_MAX + 1);
        TestOperation(test, identitySequence.IsSorted(ComparisionFunction<uint8_t>(ComparisionFunctionType::GreaterThan)));
        TestOperation(test, identitySequence.IsSorted(ComparisionFunction<uint8_t>(ComparisionFunctionType::GreaterThanEqual)));
        TestOperation(test, MakeArray<uint8_t>(1, 2, 3, 4, 5).LeftFold<uint8_t>(1, MakeFunctionT<uint8_t, uint8_t, uint8_t>([](uint8_t a, uint8_t b) -> uint8_t {
            return a * b;
        })) == 120);
        TestOperation(test, MakeArray<uint8_t>(1, 2, 3, 4, 5).RightFold<uint8_t>(1, MakeFunctionT<uint8_t, uint8_t, uint8_t>([](uint8_t a, uint8_t b) -> uint8_t {
            return a * b;
        })) == 120);

        const Function<bool, Array<uint8_t>&, const Function<bool, uint8_t, uint8_t>&>& bubbleSort = MakeFunctionT<bool, Array<uint8_t>&, const Function<bool, uint8_t, uint8_t>&>([](Array<uint8_t>& sorted, const Function<bool, uint8_t, uint8_t>& compare) -> bool {
            return sorted.BubbleSort(compare);
        });
        TestSort(test, identitySequence, bubbleSort, ComparisionFunctionType::LessThan);
        TestSort(test, identitySequence, bubbleSort, ComparisionFunctionType::LessThanEqual);
        TestSort(test, identitySequence, bubbleSort, ComparisionFunctionType::GreaterThan);
        TestSort(test, identitySequence, bubbleSort, ComparisionFunctionType::GreaterThanEqual);
        const Function<bool, Array<uint8_t>&, const Function<bool, uint8_t, uint8_t>&>& stoogeSort = MakeFunctionT<bool, Array<uint8_t>&, const Function<bool, uint8_t, uint8_t>&>([](Array<uint8_t>& sorted, const Function<bool, uint8_t, uint8_t>& compare) -> bool {
            return sorted.StoogeSort(compare);
        });
        TestSort(test, identitySequence, stoogeSort, ComparisionFunctionType::LessThan);
        TestSort(test, identitySequence, stoogeSort, ComparisionFunctionType::LessThanEqual);
        TestSort(test, identitySequence, stoogeSort, ComparisionFunctionType::GreaterThan);
        TestSort(test, identitySequence, stoogeSort, ComparisionFunctionType::GreaterThanEqual);
        const Function<bool, Array<uint8_t>&, const Function<bool, uint8_t, uint8_t>&>& oddEvenSort = MakeFunctionT<bool, Array<uint8_t>&, const Function<bool, uint8_t, uint8_t>&>([](Array<uint8_t>& sorted, const Function<bool, uint8_t, uint8_t>& compare) -> bool {
            return sorted.OddEvenSort(compare);
        });
        TestSort(test, identitySequence, oddEvenSort, ComparisionFunctionType::LessThan);
        TestSort(test, identitySequence, oddEvenSort, ComparisionFunctionType::LessThanEqual);
        TestSort(test, identitySequence, oddEvenSort, ComparisionFunctionType::GreaterThan);
        TestSort(test, identitySequence, oddEvenSort, ComparisionFunctionType::GreaterThanEqual);
        const Function<bool, Array<uint8_t>&, const Function<bool, uint8_t, uint8_t>&>& cocktailShakerSort = MakeFunctionT<bool, Array<uint8_t>&, const Function<bool, uint8_t, uint8_t>&>([](Array<uint8_t>& sorted, const Function<bool, uint8_t, uint8_t>& compare) -> bool {
            return sorted.CocktailShakerSort(compare);
        });
        TestSort(test, identitySequence, cocktailShakerSort, ComparisionFunctionType::LessThan);
        TestSort(test, identitySequence, cocktailShakerSort, ComparisionFunctionType::LessThanEqual);
        TestSort(test, identitySequence, cocktailShakerSort, ComparisionFunctionType::GreaterThan);
        TestSort(test, identitySequence, cocktailShakerSort, ComparisionFunctionType::GreaterThanEqual);
        const Function<bool, Array<uint8_t>&, const Function<bool, uint8_t, uint8_t>&>& insertationSort = MakeFunctionT<bool, Array<uint8_t>&, const Function<bool, uint8_t, uint8_t>&>([](Array<uint8_t>& sorted, const Function<bool, uint8_t, uint8_t>& compare) -> bool {
            return sorted.InsertationSort(compare);
        });
        TestSort(test, identitySequence, insertationSort, ComparisionFunctionType::LessThan);
        TestSort(test, identitySequence, insertationSort, ComparisionFunctionType::LessThanEqual);
        TestSort(test, identitySequence, insertationSort, ComparisionFunctionType::GreaterThan);
        TestSort(test, identitySequence, insertationSort, ComparisionFunctionType::GreaterThanEqual);

        TestOperation(test, IdentityCipher().TestEncryption<uint8_t>(identitySequence, CipherKey()));
        TestOperation(test, CaesarCipher().TestEncryption<uint8_t>(identitySequence, CipherKey(ByteArray::ToByteArray<ssize_t>(3))));
        TestOperation(test, ROT13().TestEncryption<uint8_t>(identitySequence, CipherKey()));
        TestOperation(test, NullCipher().TestEncryption<uint8_t>(identitySequence, CipherKey(MakeArray<CipherKey>(CipherKey(ByteArray::ToByteArray<size_t>(5)), CipherKey(ByteArray::ToByteArray<size_t>(MakeArray<size_t>(2)))))));
        TestOperation(test, AtbashCipher().TestEncryption<uint8_t>(identitySequence, CipherKey(ByteArray::ToByteArray<ssize_t>(1))));
        TestOperation(test, PolyalphabeticCipher().TestEncryption<uint8_t>(identitySequence, CipherKey(IdentitySequence<uint8_t>(7))));
        TestOperation(test, Deflate().TestEncryption<uint8_t>(identitySequence, CipherKey()));
        TestOperation(test, ZLib().TestEncryption<uint8_t>(identitySequence, CipherKey()));
        TestOperation(test, GZIP().TestEncryption<uint8_t>(identitySequence, CipherKey()));
        TestOperation(test, PackBits().TestEncryption<uint8_t>(identitySequence, CipherKey()));
        TestOperation(test, BytePairEncoding().TestEncryption<uint8_t>(identitySequence, CipherKey()));
        TestOperation(test, UnaryCode().TestEncryption<uint8_t>(identitySequence, CipherKey(MakeArray<uint8_t>(false))));
        TestOperation(test, UnaryCode().TestEncryption<uint8_t>(identitySequence, CipherKey(MakeArray<uint8_t>(true))));
        TestOperation(test, RepetitionCode().TestEncryption<uint8_t>(identitySequence, CipherKey(ByteArray::ToByteArray<size_t>(3))));
        TestOperation(test, RunLengthEncoding().TestEncryption<uint8_t>(identitySequence, CipherKey(MakeArray<uint8_t>(false))));
        TestOperation(test, RunLengthEncoding().TestEncryption<uint8_t>(identitySequence, CipherKey(MakeArray<uint8_t>(true))));
        TestOperation(test, DeltaEncoding().TestEncryption<uint8_t>(identitySequence, CipherKey()));
        TestOperation(test, TAR().TestEncryption<uint8_t>(identitySequence, CipherKey()));
        TestOperation(test, GNUTAR().TestEncryption<uint8_t>(identitySequence, CipherKey()));
        TestOperation(test, UnixStandardTAR().TestEncryption<uint8_t>(identitySequence, CipherKey()));
        TestOperation(test, Ar().TestEncryption<uint8_t>(identitySequence, CipherKey()));
        TestOperation(test, PKCS7().TestEncryption<uint8_t>(identitySequence, CipherKey(MakeArray<uint8_t>(64))));
        TestOperation(test, SHAPadding().TestEncryption<uint8_t>(identitySequence, CipherKey()));

        TestOperation(test, SHA1().IsEncrypted<uint8_t>(EmptySequence<uint8_t>(), MakeArray<uint8_t>(0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d, 0x32, 0x55, 0xbf, 0xef, 0x95, 0x60, 0x18, 0x90, 0xaf, 0xd8, 0x07, 0x09), CipherKey(MakeArray<uint8_t>(false))));
        TestOperation(test, SHA1().IsEncrypted<char>("The quick brown fox jumps over the lazy dog"_M, MakeArray<uint8_t>(0x2f, 0xd4, 0xe1, 0xc6, 0x7a, 0x2d, 0x28, 0xfc, 0xed, 0x84, 0x9e, 0xe1, 0xbb, 0x76, 0xe7, 0x39, 0x1b, 0x93, 0xeb, 0x12), CipherKey(MakeArray<uint8_t>(false))));
        TestOperation(test, SHA1().IsEncrypted<char>("The quick brown fox jumps over the lazy cog"_M, MakeArray<uint8_t>(0xde, 0x9f, 0x2c, 0x7f, 0xd2, 0x5e, 0x1b, 0x3a, 0xfa, 0xd3, 0xe8, 0x5a, 0x0b, 0xd1, 0x7d, 0x9b, 0x10, 0x0d, 0xb4, 0xb3), CipherKey(MakeArray<uint8_t>(false))));
        TestOperation(test, SHA256().IsEncrypted<uint8_t>(EmptySequence<uint8_t>(), MakeArray<uint8_t>(0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14, 0x9a, 0xfb, 0xf4, 0xc8, 0x99, 0x6f, 0xb9, 0x24, 0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c, 0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55), CipherKey(MakeArray<uint8_t>(false))));
        TestOperation(test, SHA256().IsEncrypted<uint8_t>(EmptySequence<uint8_t>(), MakeArray<uint8_t>(0xd1, 0x4a, 0x02, 0x8c, 0x2a, 0x3a, 0x2b, 0xc9, 0x47, 0x61, 0x02, 0xbb, 0x28, 0x82, 0x34, 0xc4, 0x15, 0xa2, 0xb0, 0x1f, 0x82, 0x8e, 0xa6, 0x2a, 0xc5, 0xb3, 0xe4, 0x2f), CipherKey(MakeArray<uint8_t>(true))));
        TestOperation(test, SHA256().IsEncrypted<char>("The quick brown fox jumps over the lazy dog"_M, MakeArray<uint8_t>(0x73, 0x0e, 0x10, 0x9b, 0xd7, 0xa8, 0xa3, 0x2b, 0x1c, 0xb9, 0xd9, 0xa0, 0x9a, 0xa2, 0x32, 0x5d, 0x24, 0x30, 0x58, 0x7d, 0xdb, 0xc0, 0xc3, 0x8b, 0xad, 0x91, 0x15, 0x25), CipherKey(MakeArray<uint8_t>(true))));
        TestOperation(test, SHA256().IsEncrypted<char>("The quick brown fox jumps over the lazy dog."_M, MakeArray<uint8_t>(0x61, 0x9c, 0xba, 0x8e, 0x8e, 0x05, 0x82, 0x6e, 0x9b, 0x8c, 0x51, 0x9c, 0x0a, 0x5c, 0x68, 0xf4, 0xfb, 0x65, 0x3e, 0x8a, 0x3d, 0x8a, 0xa0, 0x4b, 0xb2, 0xc8, 0xcd, 0x4c), CipherKey(MakeArray<uint8_t>(true))));
        return test;
    }
}