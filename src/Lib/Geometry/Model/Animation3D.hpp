#ifndef MathLib_Geometry_Model_Animation3D_H
#define MathLib_Geometry_Model_Animation3D_H
#include "Model3D.hpp"

namespace MathLib {
    template <typename T>
    struct Animation3D : Collection<Model3D<T>>, Printable {
        Array<Model3D<T>> frames;

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
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override {
            const String padd = CollectionToString(padding);
            const String padd2 = padd + '\t';
            String ret = "{\n";
            for (const Model3D<T>& frame : frames) ret += padd2 + frame.ToString(padd2) + '\n';
            return ret + padd + '}';
        }
    };
}

#endif