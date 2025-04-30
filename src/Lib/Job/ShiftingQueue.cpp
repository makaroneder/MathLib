#include "ShiftingQueue.hpp"

namespace MathLib {
    ShiftingQueue::ShiftingQueue(JobArray* base, ssize_t shift) : base(base), shift(shift) {
        EmptyBenchmark
    }
    ShiftingQueue::~ShiftingQueue(void) {
        StartBenchmark
        delete base;
        EndBenchmark
    }
    bool ShiftingQueue::IsEmpty(void) const {
        StartAndReturnFromBenchmark(base->IsEmpty());
    }
    bool ShiftingQueue::Add(Job* job) {
        StartAndReturnFromBenchmark(base->Add(job));
    }
    QueueState ShiftingQueue::RunQueueInternal(void) {
        StartBenchmark
        const QueueState ret = base->RunQueueInternal();
        const size_t size = base->jobs.GetSize();
        MathLib::Array<Job*> jobs = MathLib::Array<Job*>(size);
        for (size_t i = 0; i < size; i++) {
            const ssize_t index = (ssize_t)(shift + i) % (ssize_t)size;
            jobs.At(index < 0 ? index + size : index) = base->jobs.At(i);
        }
        base->jobs = jobs;
        ReturnFromBenchmark(ret);
    }
}