#ifndef Animation3D_H
#define Animation3D_H
#include "Model3D.hpp"

template <typename T>
struct Animation3D : MathLib::Collection<Model3D<T>>, MathLib::Printable {
    MathLib::Array<Model3D<T>> frames;

    [[nodiscard]] virtual size_t GetSize(void) const override {
        return frames.GetSize();
    }
    [[nodiscard]] virtual bool Add(const Model3D<T>& frame) override {
        return frames.Add(frame);
    }
    [[nodiscard]] virtual bool Reset(void) override {
        return frames.Reset();
    }
    [[nodiscard]] virtual Model3D<T>* GetValue(void) override {
        return frames.GetValue();
    }
    [[nodiscard]] virtual const Model3D<T>* GetValue(void) const override {
        return frames.GetValue();
    }
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override {
        const MathLib::String padd = MathLib::CollectionToString(padding);
        const MathLib::String padd2 = padd + '\t';
        MathLib::String ret = "{\n";
        for (const Model3D<T>& frame : frames) ret += padd2 + frame.ToString(padd2) + '\n';
        return ret + padd + '}';
    }
};

#endif