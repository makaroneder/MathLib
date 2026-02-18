#ifndef MathLib_Job_H
#define MathLib_Job_H
#include "JobState.hpp"
#include "../Allocator/Allocatable.hpp"

namespace MathLib {
    struct Job : Allocatable {
        Job(void);
        [[nodiscard]] size_t GetState(void) const;
        [[nodiscard]] JobState Step(void);
        [[nodiscard]] bool Run(void);

        protected:
        [[nodiscard]] virtual JobState RunInternal(void) = 0;

        private:
        size_t state;
    };
}

#endif