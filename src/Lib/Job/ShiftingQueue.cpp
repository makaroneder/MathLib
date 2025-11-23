#include "ShiftingQueue.hpp"

namespace MathLib {
    ShiftingQueue::ShiftingQueue(JobArray* base, ssize_t shift) : base(base), shift(shift) {}
    ShiftingQueue::~ShiftingQueue(void) {
        delete base;
    }
    bool ShiftingQueue::IsEmpty(void) const {
        return base->IsEmpty();
    }
    bool ShiftingQueue::Add(Job* job) {
        return base->Add(job);
    }
    QueueState ShiftingQueue::RunQueueInternal(void) {
        const QueueState ret = base->RunQueueInternal();
        const size_t size = base->jobs.GetSize();
        Array<Job*> jobs = Array<Job*>(size);
        for (size_t i = 0; i < size; i++) {
            const ssize_t index = (ssize_t)(shift + i) % (ssize_t)size;
            jobs.AtUnsafe(index < 0 ? index + size : index) = base->jobs.AtUnsafe(i);
        }
        base->jobs = jobs;
        return ret;
    }
}