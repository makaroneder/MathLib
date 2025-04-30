#ifndef MathLib_Job_Array_H
#define MathLib_Job_Array_H
#include "Queue.hpp"
#include "../Typedefs.hpp"

namespace MathLib {
    struct JobArray : Queue {
        virtual ~JobArray(void) override;
        [[nodiscard]] virtual bool IsEmpty(void) const override;
        [[nodiscard]] virtual bool Add(Job* job) override;

        Array<Job*> jobs;
    };
}

#endif