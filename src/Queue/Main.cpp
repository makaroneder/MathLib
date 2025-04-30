#include <Job/ShiftingQueue.hpp>
#include <Job/QueueSystem.hpp>
#include <Job/FIFOQueue.hpp>
#include <iostream>

size_t uniqueID = 0;
struct Counting : MathLib::Job {
    Counting(void) : count(0), id(uniqueID++) {}
    [[nodiscard]] virtual MathLib::JobState RunInternal(void) override {
        const bool done = ++count == 2 *(GetState() + 1);
        std::cout << id << ": " << count;
        if (done) std::cout << " (done)";
        std::cout << std::endl;
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
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
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
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}