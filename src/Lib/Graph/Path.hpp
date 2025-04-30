#ifndef MathLib_Graph_Path_H
#define MathLib_Graph_Path_H
#include "Edge.hpp"

namespace MathLib {
    struct Path : Collection<size_t>, Printable {
        [[nodiscard]] virtual size_t GetSize(void) const override;
        [[nodiscard]] virtual size_t At(size_t index) const override;
        [[nodiscard]] virtual size_t& At(size_t index) override;
        [[nodiscard]] virtual bool Add(const size_t& value) override;
        [[nodiscard]] virtual bool Reset(void) override;
        [[nodiscard]] virtual const size_t* GetValue(void) const override;
        [[nodiscard]] size_t GetEnd(void) const;
        [[nodiscard]] Edge GetEdge(void) const;
        [[nodiscard]] bool IsCyclic(void) const;
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
        [[nodiscard]] bool operator==(const Path& other) const;
        [[nodiscard]] bool operator!=(const Path& other) const;

        private:
        Array<size_t> vertices;
    };
}

#endif