#ifndef SignaledWritableSequence_H
#define SignaledWritableSequence_H
#include <Interfaces/Sequence/WritableSequence.hpp>

template <typename T>
struct SignaledWritableSequence : MathLib::WritableSequence<T> {
    SignaledWritableSequence(MathLib::WritableSequence<T>& base, const MathLib::Function<bool, size_t, T>& function) : base(base), function(function) {}
    [[nodiscard]] virtual size_t GetSize(void) const override {
        return base.GetSize();
    }
    [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
        return base.AtUnsafe(index);
    }
    [[nodiscard]] virtual bool SetUnsafe(size_t index, const T& value) override {
        return base.SetUnsafe(index, value) && function(index, value);
    }
    [[nodiscard]] virtual bool Add(const T& value) override {
        return base.Add(value);
    }
    [[nodiscard]] virtual bool Reset(void) override {
        return base.Reset();
    }

    private:
    MathLib::WritableSequence<T>& base;
    const MathLib::Function<bool, size_t, T>& function;
};

#endif