#include <FileSystem/FileSystem.hpp>
#include <Tests/Test.hpp>
#include <Logger.hpp>
#include <Host.hpp>

void Main(int, char**, MathLib::FileSystem&) {
    const MathLib::num_t start = MathLib::GetTime();
    const MathLib::Test test = MathLib::TestSelf();
    LogString(test.ToString());
    LogChar('\n');
    LogString("Time: ");
    LogString(MathLib::ToString(MathLib::GetTime() - start));
    LogChar('\n');
    if (!test.Passed()) MathLib::Panic("Some tests failed");
}