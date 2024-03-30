#ifndef NeuralNetwork_H
#define NeuralNetwork_H
#include "Matrix.hpp"

template <typename T>
struct NeuralNetwork : Printable, Saveable {
    NeuralNetwork<T>(void) {
        count = 0;
    }
    NeuralNetwork<T>(const std::vector<size_t> arch) {
        const size_t c = arch.size();
        if (c == 0) throw std::runtime_error("Invalid architecture size");
        count = c - 1;
        as.push_back(Matrix<T>(arch.at(0), 1));
        for (size_t i = 1; i < c; i++) {
            ws.push_back(Matrix<T>(arch.at(i), as.at(i - 1).GetWidth()));
            bs.push_back(Matrix<T>(arch.at(i), 1));
            as.push_back(Matrix<T>(arch.at(i), 1));
        }
    }
    constexpr size_t GetCount(void) const {
        return count;
    }
    constexpr Matrix<T>& GetInput(void) {
        return as.at(0);
    }
    constexpr Matrix<T> GetInput(void) const {
        return as.at(0);
    }
    constexpr Matrix<T>& GetOutput(void) {
        return as.at(count);
    }
    constexpr Matrix<T> GetOutput(void) const {
        return as.at(count);
    }
    constexpr void Fill(T x) {
        GetOutput().Fill(x);
        for (size_t i = 0; i < count; i++) {
            ws.at(i).Fill(x);
            bs.at(i).Fill(x);
            as.at(i).Fill(x);
        }
    }
    void Random(T min, T max) {
        for (size_t i = 0; i < count; i++) {
            ws.at(i).Random(min, max);
            bs.at(i).Random(min, max);
        }
    }
    constexpr void Forward(void) {
        for (size_t i = 0; i < count; i++) {
            as.at(i + 1) = as.at(i) * ws.at(i) + bs.at(i);
            as.at(i + 1).Sigmoid();
        }
    }
    constexpr T Cost(Matrix<T> input, Matrix<T> output) {
        if (input.GetHeight() != output.GetHeight()) return NAN;
        if (output.GetWidth() != GetOutput().GetWidth()) return NAN;
        T ret = 0;
        for (size_t y = 0; y < input.GetHeight(); y++) {
            GetInput() = input.GetRow(y);
            Forward();
            for (size_t x = 0; x < output.GetWidth(); x++) ret += std::pow(GetOutput().At(x, 0) - output.At(x, y), 2);
        }
        return ret / input.GetHeight();
    }
    constexpr NeuralNetwork<T> FiniteDiff(Matrix<T> input, Matrix<T> output, T eps_ = eps) {
        const T cost = Cost(input, output);
        NeuralNetwork<T> ret = *this;
        for (size_t i = 0; i < count; i++) {
            for (size_t y = 0; y < ws.at(i).GetHeight(); y++) {
                for (size_t x = 0; x < ws.at(i).GetWidth(); x++) {
                    const T saved = ws.at(i).At(x, y);
                    ws.at(i).At(x, y) += eps_;
                    ret.ws.at(i).At(x, y) = (Cost(input, output) - cost) / eps_;
                    ws.at(i).At(x, y) = saved;
                }
            }
            for (size_t y = 0; y < bs.at(i).GetHeight(); y++) {
                for (size_t x = 0; x < bs.at(i).GetWidth(); x++) {
                    const T saved = bs.at(i).At(x, y);
                    bs.at(i).At(x, y) += eps_;
                    ret.bs.at(i).At(x, y) = (Cost(input, output) - cost) / eps_;
                    bs.at(i).At(x, y) = saved;
                }
            }
        }
        return ret;
    }
    constexpr NeuralNetwork<T> Backprop(Matrix<T> input, Matrix<T> output) {
        if (input.GetHeight() != output.GetHeight()) return NeuralNetwork<T>();
        if (GetOutput().GetWidth() != output.GetWidth()) return NeuralNetwork<T>();
        NeuralNetwork<T> ret = *this;
        ret.Fill(0);
        for (size_t i = 0; i < input.GetHeight(); i++) {
            GetInput() = input.GetRow(i);
            Forward();
            for (size_t j = 0; j <= count; j++) ret.as.at(j).Fill(0);
            for (size_t j = 0; j < output.GetWidth(); j++) ret.GetOutput().At(j, 0) = GetOutput().At(j, 0) - output.At(j, i);
            for (size_t l = count; l > 0; l--) {
                for (size_t j = 0; j < as.at(l).GetWidth(); j++) {
                    const T a = as.at(l).At(j, 0);
                    const T da = ret.as.at(l).At(j, 0);
                    ret.bs.at(l - 1).At(j, 0) += 2 * da * a * (1 - a);
                    for (size_t k = 0; k < as.at(l - 1).GetWidth(); k++) {
                        const T w = ws.at(l - 1).At(j, k);
                        ret.ws.at(l - 1).At(j, k) += 2 * da * a * (1 - a) * as.at(l - 1).At(k, 0);
                        ret.as.at(l - 1).At(k, 0) += 2 * da * a * (1 - a) * w;
                    }
                }
            }
        }
        for (size_t i = 0; i < ret.count; i++) {
            ret.ws.at(i) /= input.GetHeight();
            ret.bs.at(i) /= input.GetHeight();
        }
        return ret;
    }
    constexpr void Learn(NeuralNetwork<T> diff, T rate) {
        for (size_t i = 0; i < count; i++) {
            ws.at(i) -= diff.ws.at(i) * rate;
            bs.at(i) -= diff.bs.at(i) * rate;
        }
    }
    virtual std::string ToString(std::string padding = "") const override {
        std::string ret = padding + "a[0] = " + GetInput().ToString() + '\n';
        for (size_t i = 0; i < count; i++)
            ret += padding + "a[" + std::to_string(i + 1) + "] = " + as.at(i + 1).ToString() + '\n' + padding + "b[" + std::to_string(i) + "] = " + bs.at(i).ToString() + '\n' + padding + "w[" + std::to_string(i) + "] = " + ws.at(i).ToString() + '\n';
        return ret;
    }
    virtual bool Save(FILE* file) const override {
        if (fwrite(&count, sizeof(size_t), 1, file) != 1) return false;
        if (!GetInput().Save(file)) return false;
        for (size_t i = 0; i < count; i++) {
            if (!as.at(i + 1).Save(file)) return false;
            if (!ws.at(i).Save(file)) return false;
            if (!bs.at(i).Save(file)) return false;
        }
        return true;
    }
    virtual bool Load(FILE* file) override {
        if (fread(&count, sizeof(size_t), 1, file) != 1) return false;
        as.push_back(Matrix<T>());
        if (!GetInput().Load(file)) return false;
        for (size_t i = 0; i < count; i++) {
            as.push_back(Matrix<T>());
            ws.push_back(Matrix<T>());
            bs.push_back(Matrix<T>());
            if (!as.at(i + 1).Load(file)) return false;
            if (!ws.at(i).Load(file)) return false;
            if (!bs.at(i).Load(file)) return false;
        }
        return true;
    }

    private:
    size_t count;
    std::vector<Matrix<T>> ws;
    std::vector<Matrix<T>> bs;
    std::vector<Matrix<T>> as;
};

#endif