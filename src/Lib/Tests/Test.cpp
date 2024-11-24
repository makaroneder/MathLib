#include "Test.hpp"
#include "../Math/Matrix.hpp"
#include "../Math/Sigmoid.hpp"
#include "../Math/Factorial.hpp"
#include "../Physics/SIUnits.hpp"

namespace MathLib {
    bool Test::ReportRecord(const Record& record) {
        return records.Add(record);
    }
    size_t Test::GetRecordCount(void) const {
        return records.GetSize();
    }
    size_t Test::GetPassed(void) const {
        size_t ret = 0;
        for (const Record& record : records) ret += record.passed;
        return ret;
    }
    String Test::ToString(const String& padding) const {
        String ret;
        for (const Record& record : records) ret += record.ToString(padding) + '\n';
        return ret;
    }
    Test TestSelf(void) {
        Test test;
        TestOperation(test, IsNaN(MakeNaN()));
        TestOperation(test, IsInf(MakeInf()));
        TestOperation(test, Kilometre<num_t>(1) == Metre<num_t>(1000));
        TestOperation(test, (Second<num_t>(1) * Second<num_t>(1)) == Second<num_t>(1).Pow(2));
        TestOperation(test, FloatsEqual<num_t>(Sigmoid<num_t>(0), 0.5));
        TestOperation(test, FloatsEqual<num_t>(Factorial<num_t>(0, 1), 1));
        TestOperation(test, Matrix<num_t>(1, 1, MakeArrayFromSingle<num_t>(2)).Normalize().GetLength() == 1);
        TestOperation(test, !Matrix<num_t>(1, 1, MakeArrayFromSingle<num_t>(0)).Normalize().GetLength());
        TestOperation(test, !Matrix<num_t>(1, 1, MakeArrayFromSingle<num_t>(0)).GetInverse().HasValue());
        TestOperation(test, Matrix<num_t>(1, 1, MakeArrayFromSingle<num_t>(1)).GetInverse().HasValue());
        TestOperation(test, IsUpper('A'));
        TestOperation(test, IsLower('a'));
        TestOperation(test, IsAlpha('a'));
        TestOperation(test, IsDigit('0'));
        TestOperation(test, IsHexDigit('f'));
        TestOperation(test, IsAlphaDigit('f'));
        TestOperation(test, FloatsEqual<num_t>(Pow(1, 0), 1));
        TestOperation(test, FloatsEqual<num_t>(Sqrt(1), 1));
        TestOperation(test, FloatsEqual<num_t>(NaturalLog(1), 0));
        TestOperation(test, FloatsEqual<num_t>(NaturalLog(e), 1));
        TestOperation(test, FloatsEqual<num_t>(Sign<num_t>(1), 1));
        TestOperation(test, FloatsEqual<num_t>(Sign<num_t>(-1), -1));
        TestOperation(test, FloatsEqual<num_t>(Sign<num_t>(0), 0));
        TestOperation(test, FloatsEqual<num_t>(LinearToGamma<num_t>(1), 1));
        TestOperation(test, FloatsEqual<num_t>(LinearToGamma<num_t>(-1), 0));
        TestOperation(test, FloatsEqual<num_t>(Abs(-1), 1));
        TestOperation(test, FloatsEqual<num_t>(Abs(complex_t(0, -1)), 1));
        TestOperation(test, FloatsEqual<num_t>(Max(1, 2), 2));
        TestOperation(test, FloatsEqual<num_t>(Min(1, 2), 1));
        TestOperation(test, FloatsEqual<num_t>(Exp(0), 1));
        TestOperation(test, FloatsEqual<num_t>(Exp(1), e));
        TestOperation(test, FloatsEqual<num_t>(Round(3.5), 4));
        TestOperation(test, FloatsEqual<num_t>(Ceil(3.1), 4));
        TestOperation(test, FloatsEqual<num_t>(Floor(3.1), 3));
        TestOperation(test, FloatsEqual<num_t>(Sin(0), 0));
        TestOperation(test, FloatsEqual<num_t>(Cos<num_t>(0), 1));
        TestOperation(test, FloatsEqual<num_t>(InversedSin(0), 0));
        TestOperation(test, FloatsEqual<num_t>(InversedCos<num_t>(1), 0));
        return test;
    }
}