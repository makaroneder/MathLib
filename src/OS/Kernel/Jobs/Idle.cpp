#include "Idle.hpp"

MathLib::JobState Idle::RunInternal(void) {
    return MathLib::JobState::Running;
}