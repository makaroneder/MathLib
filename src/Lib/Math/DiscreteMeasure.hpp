#ifndef MathLib_Math_DiscreteMeasure_H
#define MathLib_Math_DiscreteMeasure_H
#include "Set.hpp"
#include "Log.hpp"

namespace MathLib {
    template <typename T>
    struct DiscreteMeasure {
        Set<T> events;

        DiscreteMeasure(const Set<T>& events) : events(events) {
            EmptyBenchmark
        }
        size_t GetEventCount(void) const {
            StartAndReturnFromBenchmark(events.GetSize());
        }
        T GetMeasure(const Set<T>& event) const {
            StartBenchmark
            T ret = 0;
            for (const T& x : event)
                if (events.Contains(x)) ret += MassFunction(x);
            ReturnFromBenchmark(ret);
        }
        Expected<T> GetConditionalMeasure(const Set<T>& event, const Set<T>& givenBy) const {
            StartBenchmark
            const Expected<Set<T>> tmp = event * givenBy;
            if (!tmp.HasValue()) ReturnFromBenchmark(Expected<T>());
            const T div = GetMeasure(givenBy);
            ReturnFromBenchmark(FloatsEqual<T>(div, 0) ? Expected<T>() : GetMeasure(tmp.Get()) / div);
        }
        T GetExclusiveMeasure(const Set<T>& event1, const Set<T>& event2) const {
            StartAndReturnFromBenchmark(GetMeasure(event1) + GetMeasure(event2));
        }
        T GetComplementMeasure(const Set<T>& event) const {
            StartAndReturnFromBenchmark(1 - GetMeasure(event));
        }
        Expected<bool> AreIndependent(const Set<T>& event1, const Set<T>& event2) const {
            StartBenchmark
            const Expected<Set<T>> tmp = event1 * event2;
            ReturnFromBenchmark(tmp.HasValue() ? FloatsEqual<T>(GetMeasure(tmp.Get()), GetMeasure(event1) * GetMeasure(event2)) : Expected<bool>());
        }
        Expected<bool> AreExclusive(const Set<T>& event1, const Set<T>& event2) const {
            StartBenchmark
            const Expected<Set<T>> tmp = event1 * event2;
            ReturnFromBenchmark(tmp.HasValue() ? !tmp.Get().GetSize() : Expected<bool>());
        }
        Expected<bool> AreCollectivelyExhaustive(const Set<T>& event1, const Set<T>& event2) const {
            StartBenchmark
            const Expected<Set<T>> tmp = event1 + event2;
            ReturnFromBenchmark(tmp.HasValue() ? tmp.Get() == events : Expected<bool>());
        }
        Expected<Set<T>> Negate(const Set<T>& event) const {
            StartAndReturnFromBenchmark(event.Negate(events));
        }
        T GetExpectedValue(void) const {
            StartBenchmark
            T ret = 0;
            for (const T& event : events) ret += event * MassFunction(event);
            ReturnFromBenchmark(ret);
        }
        T GetExpectedValue(const Function<T, T>& function) const {
            StartBenchmark
            T ret = 0;
            for (const T& event : events) ret += function(event) * MassFunction(event);
            ReturnFromBenchmark(ret);
        }
        T GetVariance(void) const {
            StartBenchmark
            T ret = 0;
            for (const T& event : events) {
                const T tmp = event * MassFunction(event);
                ret += tmp * (event - tmp);
            }
            ReturnFromBenchmark(ret);
        }
        T GetSelfInformation(const T& x, const T& base) const {
            StartAndReturnFromBenchmark(-Log(base, MassFunction(x)));
        }
        T GetEntropy(const Set<T>& data, const T& base) const {
            StartBenchmark
            T ret = 0;
            for (const T& x : data) {
                const T tmp = MassFunction(x);
                ret += FloatsEqual<T>(tmp, 0) ? 0 : (tmp * Log(base, tmp));
            }
            ReturnFromBenchmark(-ret);
        }
        T GetTotalEntropy(const T& base) const {
            StartAndReturnFromBenchmark(GetEntropy(events, base));
        }
        virtual T MassFunction(const T& x) const = 0;
    };
}

#endif