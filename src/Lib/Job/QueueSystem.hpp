#ifndef MathLib_QueueSystem_H
#define MathLib_QueueSystem_H
#include "Queue.hpp"
#include "../Graph/Graph.hpp"

namespace MathLib {
    struct QueueSystem : Queue {
        QueueSystem(size_t inputQueue);
        QueueSystem(const Graph& graph, const Sequence<Queue*>& queue, size_t inputQueue);
        virtual ~QueueSystem(void) override;
        [[nodiscard]] bool Add(const Edge& edge);
        [[nodiscard]] bool Add(Queue* q);
        [[nodiscard]] virtual bool IsEmpty(void) const override;
        [[nodiscard]] virtual bool Add(Job* job) override;
        [[nodiscard]] virtual QueueState RunQueueInternal(void) override;

        private:
        [[nodiscard]] QueueState RunCurrentQueue(size_t current, WritableSequence<size_t>& visited);

        Graph graph;
        Array<Queue*> queue;
        size_t inputQueue;
    };
}

#endif