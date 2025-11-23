#ifndef Thread_H
#define Thread_H
#include <Interfaces/Function.hpp>
#include <thread>

template <typename... Args>
struct Thread {
    void Run(const MathLib::Function<void, Args...>& func, Args... args) {
        thread = std::thread([&func, args...](void) -> void {
            func(args...);
        });
    }
    void Join(void) {
        thread.join();
    }

    private:
    std::thread thread;
};

#endif