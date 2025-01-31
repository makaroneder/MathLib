#ifndef DiscreteProbabilitySpace_H
#define DiscreteProbabilitySpace_H
#include <Math/Set.hpp>

template <typename T, typename Number>
struct DiscreteProbabilitySpace {
    DiscreteProbabilitySpace(const MathLib::Set<T>& events) : events(events) {}
    bool IsValid(void) const {
        return events.GetSize() && MathLib::FloatsEqual<Number>(GetProbability(events), 1) && MathLib::FloatsEqual<Number>(GetProbability(MathLib::Set<T>()), 0);
    }
    size_t GetEventCount(void) const {
        return events.GetSize();
    }
    Number GetProbability(const MathLib::Set<T>& event) const {
        Number ret = Number();
        const MathLib::Array<T> data = event.GetData();
        for (const T& x : data)
            if (events.Contains(x)) ret += ProbabilityMassFunction(x);
        return ret;
    }
    MathLib::Expected<Number> GetConditionalProbability(const MathLib::Set<T>& event, const MathLib::Set<T>& givenBy) const {
        const MathLib::Expected<MathLib::Set<T>> tmp = event * givenBy;
        if (!tmp.HasValue()) return MathLib::Expected<Number>();
        const Number div = GetProbability(givenBy);
        return MathLib::FloatsEqual<Number>(div, 0) ? MathLib::Expected<Number>() : MathLib::Expected<Number>(GetProbability(tmp.Get()) / div);
    }
    Number GetExclusiveProbability(const MathLib::Set<T>& event1, const MathLib::Set<T>& event2) const {
        return GetProbability(event1) + GetProbability(event2);
    }
    Number GetComplementProbability(const MathLib::Set<T>& event) const {
        return 1 - GetProbability(event);
    }
    MathLib::Expected<bool> AreIndependent(const MathLib::Set<T>& event1, const MathLib::Set<T>& event2) const {
        const MathLib::Expected<MathLib::Set<T>> tmp = event1 * event2;
        return tmp.HasValue() ? MathLib::Expected<bool>(MathLib::FloatsEqual<Number>(GetProbability(tmp.Get()), GetProbability(event1) * GetProbability(event2))) : MathLib::Expected<bool>();
    }
    MathLib::Expected<bool> AreExclusive(const MathLib::Set<T>& event1, const MathLib::Set<T>& event2) const {
        const MathLib::Expected<MathLib::Set<T>> tmp = event1 * event2;
        return tmp.HasValue() ? MathLib::Expected<bool>(!tmp.Get().GetSize()) : MathLib::Expected<bool>();
    }
    MathLib::Expected<bool> AreCollectivelyExhaustive(const MathLib::Set<T>& event1, const MathLib::Set<T>& event2) const {
        const MathLib::Expected<MathLib::Set<T>> tmp = event1 + event2;
        return tmp.HasValue() ? MathLib::Expected<bool>(tmp.Get() == events) : MathLib::Expected<bool>();
    }
    MathLib::Expected<MathLib::Set<T>> Negate(const MathLib::Set<T>& event) const {
        return event.Negate(events);
    }
    virtual Number ProbabilityMassFunction(const T& x) const = 0;

    private:
    MathLib::Set<T> events;
};

#endif