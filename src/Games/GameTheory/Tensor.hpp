#ifndef Tensor_H
#define Tensor_H
#include <String.hpp>
#include <Host.hpp>

template <typename T>
struct Tensor : MathLib::Printable, MathLib::Iteratable<T> {
    CreateOperators(Tensor<T>, T)
    Tensor(const MathLib::Array<size_t>& dimensions) : dimensions(dimensions) {
        size_t size = 1;
        for (const size_t& dim : dimensions) size *= dim;
        data = MathLib::Array<T>(size);
    }
    Tensor(const MathLib::Array<size_t>& dimensions, const MathLib::Array<T>& data) : dimensions(dimensions), data(data) {}
    void Fill(const T& x) {
        for (T& val : data) val = x;
    }
    [[nodiscard]] T& At(const MathLib::Collection<size_t>& position) {
        return data.At(ToIndex(position));
    }
    [[nodiscard]] T At(const MathLib::Collection<size_t>& position) const {
        return data.At(ToIndex(position));
    }
    [[nodiscard]] T GetSum(void) const {
        T ret = 0;
        for (const T& val : data) ret += val;
        return ret;
    }
    [[nodiscard]] MathLib::Array<size_t> GetDimensions(void) const {
        return dimensions;
    }
    [[nodiscard]] size_t GetDimension(size_t dimension) const {
        return dimensions.At(dimension);
    }
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::String& padding = "") const override {
        return padding + ToStringInternal(MathLib::Array<size_t>(), dimensions.GetSize());
    }
    [[nodiscard]] virtual MathLib::Iterator<const T> begin(void) const override {
        return data.begin();
    }
    [[nodiscard]] virtual MathLib::Iterator<const T> end(void) const override {
        return data.end();
    }
    [[nodiscard]] virtual MathLib::Iterator<T> begin(void) override {
        return data.begin();
    }
    [[nodiscard]] virtual MathLib::Iterator<T> end(void) override {
        return data.end();
    }
    [[nodiscard]] bool operator==(const Tensor<T>& other) const {
        StartBenchmark
        bool ret = dimensions == other.dimensions;
        for (size_t i = 0; i < data.GetSize() && ret; i++) ret = data.At(i) == other.data.At(i);
        ReturnFromBenchmark(ret);
    }

    private:
    void Add(const Tensor<T>& other) {
        StartBenchmark
        if (dimensions != other.dimensions) MathLib::Panic("Invalid dimension of tensors for addition");
        for (size_t i = 0; i < data.GetSize(); i++) data.At(i) += other.data.At(i);
        EndBenchmark
    }
    void Multiply(const T& scalar) {
        StartBenchmark
        for (T& val : data) val *= scalar;
        EndBenchmark
    }
    [[nodiscard]] size_t ToIndex(const MathLib::Collection<size_t>& position) const {
        size_t ret = 0;
        for (size_t i = 0; i < position.GetSize(); i++) {
            size_t tmp = position.At(i);
            for (size_t j = 0; j < i; j++) tmp *= dimensions.At(j);
            ret += tmp;
        }
        return ret;
    }
    [[nodiscard]] MathLib::String ToStringInternal(const MathLib::Array<size_t>& position, size_t level) const {
        if (!level) {
            MathLib::String pos = "e(";
            for (size_t i = 0; i < position.GetSize(); i++)
                pos += MathLib::ToString(position.At(i), 10) + (i + 1 == position.GetSize() ? ')' : ',');
            return MathLib::ToString(At(position)) + pos;
        }
        else {
            const size_t dim = dimensions.At(level - 1);
            MathLib::String ret = "";
            MathLib::Array<size_t> pos = position;
            if (!pos.Add(0)) return "";
            for (size_t i = 0; i < dim; i++) {
                pos.At(position.GetSize()) = i;
                ret += ToStringInternal(pos, level - 1) + (i + 1 == dim ? "" : " + ");
            }
            return ret;
        }
    }

    MathLib::Array<size_t> dimensions;
    MathLib::Array<T> data;
};

#endif