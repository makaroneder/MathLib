#ifndef Regression_H
#define Regression_H
#include "DataPoint.hpp"

struct Regression : MathLib::Printable {
    Regression(const RationalNumber& learningRate, const MathLib::Sequence<DataPoint>& dataSet, const MathLib::Sequence<MathLib::String>& coefficients, const MathLib::Function<Formula, const MathLib::Sequence<Formula>&, const Formula&>& function);
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
    [[nodiscard]] bool Learn(void);
    [[nodiscard]] Formula Predict(const Formula& variable) const;

    private:
    RationalNumber learningRate;
    MathLib::Dictionary<MathLib::String, Formula> weights;
    MathLib::Dictionary<MathLib::String, Formula> derivatives;
    const MathLib::Function<Formula, const MathLib::Sequence<Formula>&, const Formula&>& function;
};
Formula MakeMeanSquaredError(const MathLib::Sequence<DataPoint>& dataSet, const MathLib::Function<Formula, Formula>& function);
Formula MakePolynomial(const MathLib::Sequence<Formula>& coefficients, const Formula& variable);

#endif