#include "DiscreteSineTransform1.hpp"
#include "DiscreteSineTransform2.hpp"
#include "DiscreteSineTransform3.hpp"
#include "DiscreteSineTransform4.hpp"
#include <Interfaces/Sequence/IdentitySequence.hpp>
#include <FileSystem/FileSystem.hpp>
#include <Logger.hpp>

template <typename T>
void TestTransform(const DiscreteTransform<T>& transform, const MathLib::Sequence<T>& input) {
    LogString(MathLib::ToString(MathLib::ToString<T>(transform.Transform(input))));
    LogChar('\n');
    LogString(MathLib::ToString(MathLib::ToString<T>(transform.CheckTransformation(input))));
    LogChar('\n');
    LogString(MathLib::BoolToString(transform.TestTransformation(input)));
    LogString("\n------------------------------------------------------------\n");
}
void Main(int, char**, MathLib::FileSystem&) {
    const MathLib::IdentitySequence<MathLib::num_t> identitySequence = MathLib::IdentitySequence<MathLib::num_t>(10);
    TestTransform<MathLib::num_t>(DiscreteSineTransform1<MathLib::num_t>(), identitySequence);
    TestTransform<MathLib::num_t>(DiscreteSineTransform2<MathLib::num_t>(), identitySequence);
    TestTransform<MathLib::num_t>(DiscreteSineTransform3<MathLib::num_t>(), identitySequence);
    TestTransform<MathLib::num_t>(DiscreteSineTransform4<MathLib::num_t>(), identitySequence);
}