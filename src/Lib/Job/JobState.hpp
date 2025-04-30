#ifndef MathLib_Job_State_H
#define MathLib_Job_State_H
#include <stdint.h>

namespace MathLib {
    enum class JobState : uint8_t {
        Running,
        Done,
        Failed,
    };
}

#endif