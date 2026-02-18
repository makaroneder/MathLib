#include "Thread.hpp"
#include "Approx/ApproxEByLimit.hpp"
#include "Approx/ApproxEBySequence.hpp"
#include "MachineLearning/Regression.hpp"
#include <Libc/HostFileSystem.hpp>
#include <FunctionPointer.hpp>
#include <CSV.hpp>
#include <iostream>

void Delim(void) {
    const size_t width = 120;
    for (size_t i = 0; i < width; i++) std::cout << '-';
    std::cout << '\n';
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        // TODO: Distributed calculation
        // TODO: Calculate pi
        std::cout << "e by sequence: " << ApproxEBySequence(NaturalNumber::FromT<uint8_t>(25)) << '\n';
        Delim();
        std::cout << "e by limit: " << ApproxEByLimit(NaturalNumber::FromT<uint8_t>(140)) << '\n';

        MathLib::HostFileSystem fs;
        const MathLib::CSV csv = MathLib::CSV(fs.Open("src/TestPrograms/Stock/DVL.csv"_M, MathLib::OpenMode::Read).ReadUntil('\0')).Reduce(MathLib::MakeArray<MathLib::String>(
            "Low",
            "High"
        ));
        const size_t lowIndex = csv.GetKeyIndex("Low"_M);
        const size_t highIndex = csv.GetKeyIndex("High"_M);
        std::cout << csv << std::endl;
        // const size_t size = csv.GetHeight();
        const size_t size = 10;
        MathLib::Array<DataPoint> lowerBoundsTrainingSet = size;
        MathLib::Array<DataPoint> upperBoundsTrainingSet = size;
        for (size_t i = 0; i < size; i++) {
            const Formula tmp = Formula(RationalNumber(NaturalNumber::FromT<size_t>(i)));
            lowerBoundsTrainingSet.AtUnsafe(i) = DataPoint(tmp, RationalNumber::FromFloatingPointString(csv.At(lowIndex, i)));
            upperBoundsTrainingSet.AtUnsafe(i) = DataPoint(tmp, RationalNumber::FromFloatingPointString(csv.At(highIndex, i)));
        }

        const MathLib::FunctionPointer<Formula, const MathLib::Sequence<Formula>&, const Formula&> func = MathLib::FunctionPointer<Formula, const MathLib::Sequence<Formula>&, const Formula&>(&MakePolynomial);
        const MathLib::Array<MathLib::String> parameters = MathLib::MakeArray<MathLib::String>('a'_M, 'b'_M);
        const RationalNumber learningRate = RationalNumber(NaturalNumber::FromT<uint8_t>(1), NaturalNumber::FromT<uint8_t>(10));
        const size_t limit = 20;
        const size_t print = 10;
        Regression lowerBound = Regression(learningRate, lowerBoundsTrainingSet, parameters, func);
        Regression upperBound = Regression(learningRate, upperBoundsTrainingSet, parameters, func);
        for (size_t i = 0; i <= limit; i++) {
            if (!lowerBound.Learn()) MathLib::Panic("Failed to learn");
            if (!upperBound.Learn()) MathLib::Panic("Failed to learn");
            if (!(i % print)) {
                std::cout << "Progress: " << i << " / " << limit << std::endl;
                std::cout << "Lower bound: " << lowerBound << std::endl;
                std::cout << "Upper bound: " << upperBound << std::endl;
            }
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}