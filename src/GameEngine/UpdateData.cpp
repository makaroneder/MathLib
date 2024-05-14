#include "UpdateData.hpp"

UpdateData::UpdateData(Second<num_t> dt, size_t iter, Event ev) : deltaTime(dt), iteration(iter), event(ev) {}