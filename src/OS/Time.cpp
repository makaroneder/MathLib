#include "Time.hpp"

Timer* mainTimer = nullptr;
Timer::Timer(void) : time(0) {}
void Timer::Sleep(MathLib::num_t duration) {
    const MathLib::num_t end = time + duration;
    while (time < end) {}
}
MathLib::num_t Timer::GetTime(void) const {
    return time;
}