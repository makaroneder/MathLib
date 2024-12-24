#include "Test.hpp"
#include "../Math/Matrix.hpp"
#include "../Math/Sigmoid.hpp"
#include "../Math/Factorial.hpp"
#include "../Physics/SIUnits.hpp"
#include "../Math/Trigonometry.hpp"

namespace MathLib {
    bool Test::ReportRecord(const Record& record) {
        StartBenchmark
        ReturnFromBenchmark(records.Add(record));
    }
    bool Test::Passed(void) const {
        StartBenchmark
        size_t ret = 0;
        for (const Record& record : records) ret += record.passed;
        ReturnFromBenchmark(ret == records.GetSize());
    }
    String Test::ToString(const String& padding) const {
        StartBenchmark
        String ret;
        size_t passed = 0;
        for (const Record& record : records) {
            ret += record.ToString(padding) + '\n';
            passed += record.passed;
        }
        ReturnFromBenchmark(ret + padding + MathLib::ToString(passed) + '/' + MathLib::ToString(records.GetSize()) + " tests passed");
    }
    Test TestSelf(void) {
        StartBenchmark
        Test test;
        TestOperation(test, IsNaN(MakeNaN()));
        TestOperation(test, IsInf(MakeInf()));
        TestOperation(test, IsNaN(complex_t(MakeNaN(), 0)));
        TestOperation(test, IsInf(complex_t(MakeInf(), 0)));
        TestOperation(test, IsNaN(complex_t(0, MakeNaN())));
        TestOperation(test, IsInf(complex_t(0, MakeInf())));
        TestOperation(test, IsNaN(complex_t(MakeNaN(), MakeNaN())));
        TestOperation(test, IsInf(complex_t(MakeInf(), MakeInf())));
        TestOperation(test, FloatsEqual<num_t>(1, 1));
        TestOperation(test, FloatsEqual<num_t>(StringToNumber("2.71"), 2.71));
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
        TestOperation(test, FloatsEqual<num_t>(Max(1, 0), 1));
        TestOperation(test, FloatsEqual<num_t>(Max(0, 1), 1));
        TestOperation(test, FloatsEqual<num_t>(Min(1, 0), 0));
        TestOperation(test, FloatsEqual<num_t>(Min(0, 1), 0));
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
        TestOperation(test, FloatsEqual<num_t>(Cbrt(-1), -1));
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
        TestOperation(test, MakeArrayFromSingle<size_t>(SIZE_MAX).At(0) == SIZE_MAX);
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
        TestOperation(test, FloatsEqual<num_t>(Matrix<num_t>(1, 1, MakeArrayFromSingle<num_t>(2)).Normalize().GetLength(), 1));
        TestOperation(test, !Matrix<num_t>(1, 1, MakeArrayFromSingle<num_t>(0)).Normalize().GetLength());
        TestOperation(test, !Matrix<num_t>(1, 1, MakeArrayFromSingle<num_t>(0)).GetInverse().HasValue());
        TestOperation(test, Matrix<num_t>(1, 1, MakeArrayFromSingle<num_t>(1)).GetInverse().HasValue());
        ReturnFromBenchmark(test);
    }
}