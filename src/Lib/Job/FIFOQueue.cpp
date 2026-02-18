#include "FIFOQueue.hpp"

namespace MathLib {
    FIFOQueue::FIFOQueue(bool fifo) : fifo(fifo) {}
    QueueState FIFOQueue::RunQueueInternal(void) {
        const size_t size = jobs.GetSize();
        Job* job = jobs.At(fifo ? 0 : size - 1);
        const JobState state = job->Step();
        const QueueState ret = QueueState(job, state);
        if (state == JobState::Running) return ret;
        Array<Job*> tmp = Array<Job*>(size - 1);
        for (size_t i = 1; i < size; i++) tmp.AtUnsafe(i - 1) = jobs.AtUnsafe(i);
        jobs = tmp;
        return ret;
    }
}