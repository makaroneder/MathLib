#ifndef DiscreteMeasure_H
#define DiscreteMeasure_H
#include <Interfaces/Function.hpp>
#include <Math/Set.hpp>
#include <Math/Log.hpp>

template <typename T, typename Number>
struct DiscreteMeasure {
    DiscreteMeasure(const MathLib::Set<T>& events) : events(events) {}
    bool IsValid(void) const {
        return events.GetSize() && MathLib::FloatsEqual<Number>(GetMeasure(MathLib::Set<T>()), 0);
    }
    size_t GetEventCount(void) const {
        return events.GetSize();
    }
    Number GetMeasure(const MathLib::Set<T>& event) const {
        Number ret = Number();
        for (const T& x : event)
            if (events.Contains(x)) ret += MassFunction(x);
        return ret;
    }
    MathLib::Expected<Number> GetConditionalMeasure(const MathLib::Set<T>& event, const MathLib::Set<T>& givenBy) const {
        const MathLib::Expected<MathLib::Set<T>> tmp = event * givenBy;
        if (!tmp.HasValue()) return MathLib::Expected<Number>();
        const Number div = GetMeasure(givenBy);
        return MathLib::FloatsEqual<Number>(div, 0) ? MathLib::Expected<Number>() : MathLib::Expected<Number>(GetMeasure(tmp.Get()) / div);
    }
    Number GetExclusiveMeasure(const MathLib::Set<T>& event1, const MathLib::Set<T>& event2) const {
        return GetMeasure(event1) + GetMeasure(event2);
    }
    Number GetComplementMeasure(const MathLib::Set<T>& event) const {
        return 1 - GetMeasure(event);
    }
    MathLib::Expected<bool> AreIndependent(const MathLib::Set<T>& event1, const MathLib::Set<T>& event2) const {
        const MathLib::Expected<MathLib::Set<T>> tmp = event1 * event2;
        return tmp.HasValue() ? MathLib::Expected<bool>(MathLib::FloatsEqual<Number>(GetMeasure(tmp.Get()), GetMeasure(event1) * GetMeasure(event2))) : MathLib::Expected<bool>();
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
    T GetExpectedValue(void) const {
        T ret = T();
        for (const T& event : events) ret += event * MassFunction(event);
        return ret;
    }
    T GetExpectedValue(const MathLib::Function<T, T>& function) const {
        T ret = T();
        for (const T& event : events) ret += function(event) * MassFunction(event);
        return ret;
    }
    T GetVariance(void) const {
        T ret = T();
        for (const T& event : events) {
            const T tmp = event * MassFunction(event);
            ret += tmp * (event - tmp);
        }
        return ret;
    }
    Number GetSelfInformation(const T& x, const Number& base) const {
        return -MathLib::Log(base, MassFunction(x));
    }
    Number GetEntropy(const MathLib::Set<T>& data, const Number& base) const {
        Number ret = Number();
        for (const T& x : data) {
            const Number tmp = MassFunction(x);
            ret += MathLib::FloatsEqual<Number>(tmp, 0) ? 0 : (tmp * MathLib::Log(base, tmp));
        }
        return -ret;
    }
    Number GetTotalEntropy(const Number& base) const {
        return GetEntropy(events, base);
    }
    virtual Number MassFunction(const T& x) const = 0;

    private:
    MathLib::Set<T> events;
};

#endif