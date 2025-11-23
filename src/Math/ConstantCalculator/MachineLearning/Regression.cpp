#include "Regression.hpp"
#include <FunctionT.hpp>

Formula MakeMeanSquaredError(const MathLib::Sequence<DataPoint>& dataSet, const MathLib::Function<Formula, Formula>& function) {
    const size_t size = dataSet.GetSize();
    Formula ret = RationalNumber();
    for (size_t i = 0; i < size; i++) {
        const DataPoint data = dataSet.At(i);
        const Formula tmp = Formula::MakeSub(data.output, function(data.input));
        ret = Formula::MakeAdd(ret, Formula::MakeSquare(tmp));
    }
    return Formula::MakeDiv(ret, RationalNumber(NaturalNumber::FromT<size_t>(size)));
}
Formula MakePolynomial(const MathLib::Sequence<Formula>& coefficients, const Formula& variable) {
    const size_t size = coefficients.GetSize();
    Formula ret = RationalNumber();
    Formula mul = RationalNumber(NaturalNumber::FromT<uint8_t>(1));
    for (size_t i = size; i; i--) {
        ret = Formula::MakeAdd(ret, Formula::MakeMul(coefficients.At(i - 1), mul));
        mul = Formula::MakeMul(mul, variable);
    }
    return ret;
}
Regression::Regression(const RationalNumber& learningRate, const MathLib::Sequence<DataPoint>& dataSet, const MathLib::Sequence<MathLib::String>& coefficients, const MathLib::Function<Formula, const MathLib::Sequence<Formula>&, const Formula&>& function) : learningRate(learningRate), weights(), derivatives(), function(function) {
    const size_t size = coefficients.GetSize();
    MathLib::Array<Formula> coeffs = size;
    weights = size;
    for (size_t i = 0; i < size; i++) {
        coeffs.At(i) = coefficients.At(i);
        weights.At(i) = MathLib::DictionaryElement<MathLib::String, Formula>(coeffs.At(i).name, Formula(RationalNumber()));
    }
    MakeMeanSquaredError(dataSet, MathLib::MakeFunctionT<Formula, Formula>([&coeffs, &function](Formula variable) -> Formula {
        return function(coeffs, variable);
    })).Evaluate(derivatives);
}
MathLib::String Regression::ToString(const MathLib::Sequence<char>& padding) const {
    const MathLib::String padd = MathLib::CollectionToString(padding);
    const MathLib::String padd2 = padd + '\t';
    MathLib::String ret = "{\n"_M + padd2 + "Learning rate: " + learningRate.ToString() + '\n';
    ret += padd2 + "Weights: " + weights.ToString(padd2) + '\n';
    ret += padd2 + "Derivatives: " + derivatives.ToString(padd2) + '\n';
    return ret + padd + '}';
}
bool Regression::Learn(void) {
    MathLib::Dictionary<MathLib::String, Formula> tmp;
    for (MathLib::DictionaryElement<MathLib::String, Formula>& weight : weights) {
        const Formula derivative = derivatives.Get(weight.GetKey()).GetOr(RationalNumber()).Substitute(weights).Evaluate(tmp);
        if (derivative.type != Formula::Type::Constant) return false;
        weight.value.value -= learningRate * derivative.value;
    }
    return true;
}
Formula Regression::Predict(const Formula& variable) const {
    const size_t size = weights.GetSize();
    MathLib::Array<Formula> values = size;
    for (size_t i = 0; i < size; i++) values.At(i) = weights.At(i).value;
    return function(values, variable);
}