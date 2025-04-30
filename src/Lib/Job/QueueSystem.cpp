#include "QueueSystem.hpp"

namespace MathLib {
    QueueSystem::QueueSystem(size_t inputQueue) : graph(), queue(), inputQueue(inputQueue) {}
    QueueSystem::QueueSystem(const Graph& graph, const Sequence<Queue*>& queue, size_t inputQueue) : graph(graph), queue(CollectionToArray<Queue*>(queue)), inputQueue(inputQueue) {}
    QueueSystem::~QueueSystem(void) {
        for (Queue*& q : queue) delete q;
    }
    bool QueueSystem::Add(const Edge& edge) {
        return graph.Add(edge);
    }
    bool QueueSystem::Add(Queue* q) {
        return q && queue.Add(q);
    }
    bool QueueSystem::IsEmpty(void) const {
        for (const Queue* const& q : queue)
            if (!q->IsEmpty()) return false;
        return true;
    }
    bool QueueSystem::Add(Job* job) {
        return queue.At(inputQueue)->Add(job);
    }
    QueueState QueueSystem::RunQueueInternal(void) {
        Array<size_t> visited;
        const size_t size = queue.GetSize();
        for (size_t i = 0; i < size; i++) {
            if (queue.At(i)->IsEmpty()) continue;
            const QueueState tmp = RunCurrentQueue(i, visited);
            if (tmp.state != JobState::Running) return tmp;
        }
        return QueueState(nullptr, JobState::Running);
    }
    QueueState QueueSystem::RunCurrentQueue(size_t current, WritableSequence<size_t>& visited) {
        if (!visited.Add(current)) return QueueState(nullptr, JobState::Failed);
        QueueState ret = queue.At(current)->RunQueueInternal();
        if (ret.state == JobState::Failed) return ret;
        const Array<size_t> outputs = graph.GetConnections(current, false);
        if (ret.state == JobState::Done) {
            if (outputs.IsEmpty()) return ret;
            else if (!queue.At(outputs.At(RandomNumber<size_t>(0, outputs.GetSize() - 1)))->Add(ret.job)) return QueueState(ret.job, JobState::Failed);
        }
        if (!visited.Add(current)) return QueueState(nullptr, JobState::Failed);
        for (size_t next : outputs) {
            if (queue.At(next)->IsEmpty()) continue;
            const QueueState tmp = RunCurrentQueue(next, visited);
            if (tmp.state != JobState::Running) return tmp;
        }
        return QueueState(nullptr, JobState::Running);
    }
}