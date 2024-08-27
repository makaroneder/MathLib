#include "Time.hpp"

Timer* mainTimer = nullptr;
Timer::Timer(void) : time(0) {}
void Timer::Sleep(num_t duration) {
    const num_t end = time + duration;
    while (time < end) {}
}
num_t Timer::GetTime(void) const {
    return time;
}