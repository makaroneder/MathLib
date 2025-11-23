#ifndef MathLib_Math_DiscreteMeasure_H
#define MathLib_Math_DiscreteMeasure_H
#include "Set.hpp"
#include "Log.hpp"

namespace MathLib {
    template <typename T>
    struct DiscreteMeasure {
        Set<T> events;

        DiscreteMeasure(const Set<T>& events) : events(events) {}
        size_t GetEventCount(void) const {
            return events.GetSize();
        }
        T GetMeasure(const Set<T>& event) const {
            T ret = 0;
            for (const T& x : event)
                if (events.Contains(x)) ret += MassFunction(x);
            return ret;
        }
        Expected<T> GetConditionalMeasure(const Set<T>& event, const Set<T>& givenBy) const {
            const Expected<Set<T>> tmp = event * givenBy;
            if (!tmp.HasValue()) return Expected<T>();
            const T div = GetMeasure(givenBy);
            return FloatsEqual<T>(div, 0) ? Expected<T>() : GetMeasure(tmp.Get()) / div;
        }
        T GetExclusiveMeasure(const Set<T>& event1, const Set<T>& event2) const {
            return GetMeasure(event1) + GetMeasure(event2);
        }
        T GetComplementMeasure(const Set<T>& event) const {
            return 1 - GetMeasure(event);
        }
        Expected<bool> AreIndependent(const Set<T>& event1, const Set<T>& event2) const {
            const Expected<Set<T>> tmp = event1 * event2;
            return tmp.HasValue() ? FloatsEqual<T>(GetMeasure(tmp.Get()), GetMeasure(event1) * GetMeasure(event2)) : Expected<bool>();
        }
        Expected<bool> AreExclusive(const Set<T>& event1, const Set<T>& event2) const {
            const Expected<Set<T>> tmp = event1 * event2;
            return tmp.HasValue() ? !tmp.Get().GetSize() : Expected<bool>();
        }
        Expected<bool> AreCollectivelyExhaustive(const Set<T>& event1, const Set<T>& event2) const {
            const Expected<Set<T>> tmp = event1 + event2;
            return tmp.HasValue() ? tmp.Get() == events : Expected<bool>();
        }
        Expected<Set<T>> Negate(const Set<T>& event) const {
            return event.Negate(events);
        }
        T GetExpectedValue(void) const {
            T ret = 0;
            for (const T& event : events) ret += event * MassFunction(event);
            return ret;
        }
        T GetExpectedValue(const Function<T, T>& function) const {
            T ret = 0;
            for (const T& event : events) ret += function(event) * MassFunction(event);
            return ret;
        }
        T GetVariance(void) const {
            T ret = 0;
            for (const T& event : events) {
                const T tmp = event * MassFunction(event);
                ret += tmp * (event - tmp);
            }
            return ret;
        }
        T GetSelfInformation(const T& x, const T& base) const {
            return -Log(base, MassFunction(x));
        }
        T GetEntropy(const Set<T>& data, const T& base) const {
            T ret = 0;
            for (const T& x : data) {
                const T tmp = MassFunction(x);
                ret += FloatsEqual<T>(tmp, 0) ? 0 : (tmp * Log(base, tmp));
            }
            return -ret;
        }
        T GetTotalEntropy(const T& base) const {
            return GetEntropy(events, base);
        }
        virtual T MassFunction(const T& x) const = 0;
    };
}

#endif