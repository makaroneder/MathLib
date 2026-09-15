#include <FileSystem/FileSystem.hpp>
#include <Job/ShiftingQueue.hpp>
#include <Job/QueueSystem.hpp>
#include <Job/FIFOQueue.hpp>
#include <Logger.hpp>

size_t uniqueID = 0;
struct Counting : MathLib::Job {
    Counting(void) : count(0), id(uniqueID++) {}
    [[nodiscard]] virtual MathLib::JobState RunInternal(void) override {
        const bool done = ++count == 2 *(GetState() + 1);
        LogString(MathLib::ToString(id, 10));
        LogString(": ");
        LogString(MathLib::ToString(count, 10));
        if (done) LogString(" (done)");
        LogChar('\n');
        if (done) {
            count = 0;
            return MathLib::JobState::Done;
        }
        return MathLib::JobState::Running;
    }

    private:
    size_t count;
    size_t id;
};
void Main(int, char**, MathLib::FileSystem&) {
    MathLib::QueueSystem system = MathLib::QueueSystem(MathLib::Graph(MathLib::MakeArray<MathLib::Edge>(
        MathLib::Edge(0, 1)
    )), MathLib::MakeArray<MathLib::Queue*>(
        new MathLib::ShiftingQueue(
            new MathLib::FIFOQueue(true), -1
        ),
        new MathLib::ShiftingQueue(
            new MathLib::FIFOQueue(true), 1
        )
    ), 0);
    for (size_t i = 0; i < 10; i++)
        if (!system.Add(new Counting())) MathLib::Panic("Failed to add new job");
    if (!system.Run()) MathLib::Panic("Failed to run system");
}