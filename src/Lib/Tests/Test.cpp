#include "Test.hpp"
#include "../MinMax.hpp"
#include "../Interval.hpp"
#include "../FunctionT.hpp"
#include "../Math/Matrix.hpp"
#include "../Math/Sigmoid.hpp"
#include "../Math/Factorial.hpp"
#include "../Physics/SIUnits.hpp"
#include "../Math/Trigonometry.hpp"
#include "../Cryptography/Cipher/ROT13.hpp"
#include "../Interfaces/IdentitySequence.hpp"
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
#include "../Cryptography/Cipher/IdentityCipher.hpp"
#include "../Cryptography/Code/RunLengthEncoding.hpp"
#include "../Cryptography/Cipher/PolyalphabeticCipher.hpp"
#include "../Cryptography/Compressor/BytePairEncoding/BytePairEncoding.hpp"

// TODO: Add test cases

namespace MathLib {
    bool Test::ReportRecord(const Record& record) {
        StartAndReturnFromBenchmark(records.Add(record));
    }
    bool Test::Passed(void) const {
        StartBenchmark
        size_t ret = 0;
        for (const Record& record : records) ret += record.passed;
        ReturnFromBenchmark(ret == records.GetSize());
    }
    String Test::ToString(const Sequence<char>& padding) const {
        StartBenchmark
        String ret;
        size_t passed = 0;
        for (const Record& record : records) {
            ret += record.ToString(padding) + '\n';
            passed += record.passed;
        }
        ReturnFromBenchmark(ret + padding + MathLib::ToString(passed) + '/' + MathLib::ToString(records.GetSize()) + " tests passed");
    }
    void TestSort(Test& test, const Sequence<uint8_t>& data, const Function<bool, Array<uint8_t>&, const Function<bool, uint8_t, uint8_t>&>& sort, ComparisionFunctionType type) {
        Array<uint8_t> sorted = CollectionToArray<uint8_t>(data);
        const ComparisionFunction<uint8_t> compare = ComparisionFunction<uint8_t>(type);
        TestOperation(test, sort(sorted, compare));
        TestOperation(test, sorted.IsSorted(compare));
    }
    Test TestSelf(void) {
        StartBenchmark
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

        const Function<bool, Array<uint8_t>&, const Function<bool, uint8_t, uint8_t>&>& bubbleSort = MakeFunctionT<bool, Array<uint8_t>&, const Function<bool, uint8_t, uint8_t>&>(nullptr, [] (const void*, Array<uint8_t>& sorted, const Function<bool, uint8_t, uint8_t>& compare) -> bool {
            return sorted.BubbleSort(compare);
        });
        TestSort(test, identitySequence, bubbleSort, ComparisionFunctionType::LessThan);
        TestSort(test, identitySequence, bubbleSort, ComparisionFunctionType::LessThanEqual);
        TestSort(test, identitySequence, bubbleSort, ComparisionFunctionType::GreaterThan);
        TestSort(test, identitySequence, bubbleSort, ComparisionFunctionType::GreaterThanEqual);
        const Function<bool, Array<uint8_t>&, const Function<bool, uint8_t, uint8_t>&>& stoogeSort = MakeFunctionT<bool, Array<uint8_t>&, const Function<bool, uint8_t, uint8_t>&>(nullptr, [] (const void*, Array<uint8_t>& sorted, const Function<bool, uint8_t, uint8_t>& compare) -> bool {
            return sorted.StoogeSort(compare);
        });
        TestSort(test, identitySequence, stoogeSort, ComparisionFunctionType::LessThan);
        TestSort(test, identitySequence, stoogeSort, ComparisionFunctionType::LessThanEqual);
        TestSort(test, identitySequence, stoogeSort, ComparisionFunctionType::GreaterThan);
        TestSort(test, identitySequence, stoogeSort, ComparisionFunctionType::GreaterThanEqual);
        const Function<bool, Array<uint8_t>&, const Function<bool, uint8_t, uint8_t>&>& oddEvenSort = MakeFunctionT<bool, Array<uint8_t>&, const Function<bool, uint8_t, uint8_t>&>(nullptr, [] (const void*, Array<uint8_t>& sorted, const Function<bool, uint8_t, uint8_t>& compare) -> bool {
            return sorted.OddEvenSort(compare);
        });
        TestSort(test, identitySequence, oddEvenSort, ComparisionFunctionType::LessThan);
        TestSort(test, identitySequence, oddEvenSort, ComparisionFunctionType::LessThanEqual);
        TestSort(test, identitySequence, oddEvenSort, ComparisionFunctionType::GreaterThan);
        TestSort(test, identitySequence, oddEvenSort, ComparisionFunctionType::GreaterThanEqual);
        const Function<bool, Array<uint8_t>&, const Function<bool, uint8_t, uint8_t>&>& cocktailShakerSort = MakeFunctionT<bool, Array<uint8_t>&, const Function<bool, uint8_t, uint8_t>&>(nullptr, [] (const void*, Array<uint8_t>& sorted, const Function<bool, uint8_t, uint8_t>& compare) -> bool {
            return sorted.CocktailShakerSort(compare);
        });
        TestSort(test, identitySequence, cocktailShakerSort, ComparisionFunctionType::LessThan);
        TestSort(test, identitySequence, cocktailShakerSort, ComparisionFunctionType::LessThanEqual);
        TestSort(test, identitySequence, cocktailShakerSort, ComparisionFunctionType::GreaterThan);
        TestSort(test, identitySequence, cocktailShakerSort, ComparisionFunctionType::GreaterThanEqual);
        const Function<bool, Array<uint8_t>&, const Function<bool, uint8_t, uint8_t>&>& insertationSort = MakeFunctionT<bool, Array<uint8_t>&, const Function<bool, uint8_t, uint8_t>&>(nullptr, [] (const void*, Array<uint8_t>& sorted, const Function<bool, uint8_t, uint8_t>& compare) -> bool {
            return sorted.InsertationSort(compare);
        });
        TestSort(test, identitySequence, insertationSort, ComparisionFunctionType::LessThan);
        TestSort(test, identitySequence, insertationSort, ComparisionFunctionType::LessThanEqual);
        TestSort(test, identitySequence, insertationSort, ComparisionFunctionType::GreaterThan);
        TestSort(test, identitySequence, insertationSort, ComparisionFunctionType::GreaterThanEqual);

        TestOperation(test, IdentityCipher().TestEncryption<uint8_t>(identitySequence, Array<uint64_t>()));
        TestOperation(test, CaesarCipher().TestEncryption<uint8_t>(identitySequence, MakeArray<uint64_t>(3)));
        TestOperation(test, ROT13().TestEncryption<uint8_t>(identitySequence, Array<uint64_t>()));
        TestOperation(test, NullCipher().TestEncryption<uint8_t>(identitySequence, MakeArray<uint64_t>(5, 2)));
        TestOperation(test, AtbashCipher().TestEncryption<uint8_t>(identitySequence, MakeArray<uint64_t>(1)));
        TestOperation(test, PolyalphabeticCipher().TestEncryption<uint8_t>(identitySequence, IdentitySequence<uint64_t>(7)));
        TestOperation(test, Deflate().TestEncryption<uint8_t>(identitySequence, Array<uint64_t>()));
        TestOperation(test, ZLib().TestEncryption<uint8_t>(identitySequence, Array<uint64_t>()));
        TestOperation(test, GZIP().TestEncryption<uint8_t>(identitySequence, Array<uint64_t>()));
        TestOperation(test, PackBits().TestEncryption<uint8_t>(identitySequence, Array<uint64_t>()));
        TestOperation(test, BytePairEncoding().TestEncryption<uint8_t>(identitySequence, Array<uint64_t>()));
        TestOperation(test, UnaryCode().TestEncryption<uint8_t>(identitySequence, MakeArray<uint64_t>(false)));
        TestOperation(test, UnaryCode().TestEncryption<uint8_t>(identitySequence, MakeArray<uint64_t>(true)));
        TestOperation(test, RepetitionCode().TestEncryption<uint8_t>(identitySequence, MakeArray<uint64_t>(3)));
        TestOperation(test, RunLengthEncoding().TestEncryption<uint8_t>(identitySequence, MakeArray<uint64_t>(false)));
        TestOperation(test, RunLengthEncoding().TestEncryption<uint8_t>(identitySequence, MakeArray<uint64_t>(true)));
        TestOperation(test, DeltaEncoding().TestEncryption<uint8_t>(identitySequence, Array<uint64_t>()));
        ReturnFromBenchmark(test);
    }
}