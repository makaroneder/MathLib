#ifndef UpdateData_H
#define UpdateData_H
#include <Physics/SIUnits.hpp>
#include <Event.hpp>

struct UpdateData {
    UpdateData(Second<num_t> dt, size_t iter, Event ev);

    Second<num_t> deltaTime;
    size_t iteration;
    Event event;
};

#endif