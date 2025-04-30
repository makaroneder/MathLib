#include "FIFOQueue.hpp"

namespace MathLib {
    FIFOQueue::FIFOQueue(bool fifo) : fifo(fifo) {
        EmptyBenchmark
    }
    QueueState FIFOQueue::RunQueueInternal(void) {
        StartBenchmark
        Job* job = jobs.At(fifo ? 0 : jobs.GetSize() - 1);
        const JobState state = job->Step();
        const QueueState ret = QueueState(job, state);
        if (state == JobState::Running) ReturnFromBenchmark(ret);
        const size_t size = jobs.GetSize();
        MathLib::Array<Job*> tmp = MathLib::Array<Job*>(size - 1);
        for (size_t i = 1; i < size; i++) tmp.At(i - 1) = jobs.At(i);
        jobs = tmp;
        ReturnFromBenchmark(ret);
    }
}