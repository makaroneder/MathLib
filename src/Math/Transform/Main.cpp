#include "DiscreteSineTransform1.hpp"
#include "DiscreteSineTransform2.hpp"
#include "DiscreteSineTransform3.hpp"
#include "DiscreteSineTransform4.hpp"
#include <Interfaces/IdentitySequence.hpp>
#include <iostream>

template <typename T>
void TestTransform(const DiscreteTransform<T>& transform, const MathLib::Sequence<T>& input) {
    std::cout << MathLib::ToString<T>(transform.Transform(input)) << std::endl;
    std::cout << MathLib::ToString<T>(transform.CheckTransformation(input)) << std::endl;
    std::cout << transform.TestTransformation(input) << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        const MathLib::IdentitySequence<MathLib::num_t> identitySequence = MathLib::IdentitySequence<MathLib::num_t>(10);
        TestTransform<MathLib::num_t>(DiscreteSineTransform1<MathLib::num_t>(), identitySequence);
        TestTransform<MathLib::num_t>(DiscreteSineTransform2<MathLib::num_t>(), identitySequence);
        TestTransform<MathLib::num_t>(DiscreteSineTransform3<MathLib::num_t>(), identitySequence);
        TestTransform<MathLib::num_t>(DiscreteSineTransform4<MathLib::num_t>(), identitySequence);
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}