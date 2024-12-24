#ifndef MathLib_NeuralNetwork_H
#define MathLib_NeuralNetwork_H
#include "Math/Matrix.hpp"
#include "Math/Sigmoid.hpp"
#include "Math/Trigonometry.hpp"

namespace MathLib {
    /// @brief Neural network
    /// @tparam T Type of number
    template <typename T>
    struct NeuralNetwork : Printable, Saveable {
        enum class ActivationFunction : uint8_t {
            None,
            Sigmoid,
            Tanh,
            ReLU,
            LeakyReLU,
        } activation;
        /// @brief Creates empty neural network
        NeuralNetwork<T>(void) : activation(ActivationFunction::None) {
            EmptyBenchmark
        }
        /// @brief Creates a new neural network
        /// @param arch Architecture of the neural network
        NeuralNetwork<T>(ActivationFunction activation, const Array<size_t>& arch) : activation(activation) {
            StartBenchmark
            const size_t c = arch.GetSize();
            if (!c) Panic("Invalid architecture size");
            count = c - 1;
            if (!as.Add(Matrix<T>(arch.At(0), 1))) Panic("Failed to allocate input neuron");
            for (size_t i = 1; i < c; i++) {
                if (!ws.Add(Matrix<T>(arch.At(i), as.At(i - 1).GetWidth()))) Panic("Failed to allocate weight");
                if (!bs.Add(Matrix<T>(arch.At(i), 1))) Panic("Failed to allocate bias");
                if (!as.Add(Matrix<T>(arch.At(i), 1))) Panic("Failed to allocate neuron");
            }
            EndBenchmark
        }
        /// @brief Returns count of weights, biases and count of neurons - 1
        /// @return Count of weights, biases and count of neurons - 1
        size_t GetCount(void) const {
            StartBenchmark
            ReturnFromBenchmark(count);
        }
        /// @brief Returns input neuron
        /// @return Input neuron
        Matrix<T>& GetInput(void) {
            StartBenchmark
            ReturnFromBenchmark(as.At(0));
        }
        /// @brief Returns input neuron
        /// @return Input neuron
        Matrix<T> GetInput(void) const {
            StartBenchmark
            ReturnFromBenchmark(as.At(0));
        }
        /// @brief Returns output neuron
        /// @return Output neuron
        Matrix<T>& GetOutput(void) {
            StartBenchmark
            ReturnFromBenchmark(as.At(count));
        }
        /// @brief Returns output neuron
        /// @return Output neuron
        Matrix<T> GetOutput(void) const {
            StartBenchmark
            ReturnFromBenchmark(as.At(count));
        }
        /// @brief Fills weights, biases and neurons
        /// @param x Value to fill with
        void Fill(const T& x) {
            StartBenchmark
            for (size_t i = 0; i < count; i++) {
                ws.At(i).Fill(x);
                bs.At(i).Fill(x);
                as.At(i).Fill(x);
            }
            GetOutput().Fill(x);
            EndBenchmark
        }
        /// @brief Randomizes weights and biases
        /// @param min Minimal value
        /// @param max Maximal value
        void Random(const T& min, const T& max) {
            StartBenchmark
            for (size_t i = 0; i < count; i++) {
                ws.At(i).Random(min, max);
                bs.At(i).Random(min, max);
            }
            EndBenchmark
        }
        /// @brief Applies weights and biases to neurons
        /// @return Status 
        bool Forward(void) {
            StartBenchmark
            for (size_t i = 0; i < count; i++) {
                const Expected<Matrix<T>> tmp = as.At(i) * ws.At(i);
                if (!tmp.HasValue()) ReturnFromBenchmark(false);
                as.At(i + 1) = tmp.Get() + bs.At(i);
                for (size_t y = 0; y < as.At(i + 1).GetHeight(); y++)
                    for (size_t x = 0; x < as.At(i + 1).GetWidth(); x++)
                        as.At(i + 1).At(x, y) = Activation(as.At(i + 1).At(x, y));
            }
            ReturnFromBenchmark(true);
        }
        /// @brief Calculates average error
        /// @param input Input data
        /// @param output Output data
        /// @return Average error
        T Cost(const Matrix<T>& input, const Matrix<T>& output) {
            StartBenchmark
            if (input.GetHeight() != output.GetHeight() || output.GetWidth() != GetOutput().GetWidth()) ReturnFromBenchmark(MakeNaN());
            T ret = 0;
            for (size_t y = 0; y < input.GetHeight(); y++) {
                GetInput() = input.GetRow(y);
                if (!Forward()) ReturnFromBenchmark(MakeNaN());
                for (size_t x = 0; x < output.GetWidth(); x++) ret += Pow(GetOutput().At(x, 0) - output.At(x, y), 2);
            }
            ReturnFromBenchmark(ret / input.GetHeight());
        }
        /// @brief Calculates difference between expected and actual results using finite difference algorithm
        /// @param input Input data
        /// @param output Output data
        /// @param eps_ Error tolerance
        /// @return Difference
        NeuralNetwork<T> FiniteDiff(const Matrix<T>& input, const Matrix<T>& output, const T& eps_ = eps) {
            StartBenchmark
            const T cost = Cost(input, output);
            NeuralNetwork<T> ret = *this;
            for (size_t i = 0; i < count; i++) {
                for (size_t y = 0; y < ws.At(i).GetHeight(); y++) {
                    for (size_t x = 0; x < ws.At(i).GetWidth(); x++) {
                        const T saved = ws.At(i).At(x, y);
                        ws.At(i).At(x, y) += eps_;
                        ret.ws.At(i).At(x, y) = (Cost(input, output) - cost) / eps_;
                        ws.At(i).At(x, y) = saved;
                    }
                }
                for (size_t y = 0; y < bs.At(i).GetHeight(); y++) {
                    for (size_t x = 0; x < bs.At(i).GetWidth(); x++) {
                        const T saved = bs.At(i).At(x, y);
                        bs.At(i).At(x, y) += eps_;
                        ret.bs.At(i).At(x, y) = (Cost(input, output) - cost) / eps_;
                        bs.At(i).At(x, y) = saved;
                    }
                }
            }
            ReturnFromBenchmark(ret);
        }
        /// @brief Calculates difference between expected and actual results using backpropagation algorithm
        /// @param input Input data
        /// @param output Output data
        /// @return Difference
        Expected<NeuralNetwork<T>> Backprop(const Matrix<T>& input, const Matrix<T>& output) {
            StartBenchmark
            if (input.GetHeight() != output.GetHeight() || GetOutput().GetWidth() != output.GetWidth()) ReturnFromBenchmark(Expected<NeuralNetwork<T>>());
            NeuralNetwork<T> ret = *this;
            ret.Fill(0);
            for (size_t i = 0; i < input.GetHeight(); i++) {
                GetInput() = input.GetRow(i);
                if (!Forward()) ReturnFromBenchmark(Expected<NeuralNetwork<T>>());
                for (size_t j = 0; j <= count; j++) ret.as.At(j).Fill(0);
                for (size_t j = 0; j < output.GetWidth(); j++) ret.GetOutput().At(j, 0) = GetOutput().At(j, 0) - output.At(j, i);
                for (size_t l = count; l > 0; l--) {
                    for (size_t j = 0; j < as.At(l).GetWidth(); j++) {
                        const T a = as.At(l).At(j, 0);
                        const T da = ret.as.At(l).At(j, 0);
                        const T qa = ActivationDerivate(a);
                        ret.bs.At(l - 1).At(j, 0) += 2 * da * qa;
                        for (size_t k = 0; k < as.At(l - 1).GetWidth(); k++) {
                            const T w = ws.At(l - 1).At(j, k);
                            ret.ws.At(l - 1).At(j, k) += 2 * da * qa * as.At(l - 1).At(k, 0);
                            ret.as.At(l - 1).At(k, 0) += 2 * da * qa * w;
                        }
                    }
                }
            }
            for (size_t i = 0; i < ret.count; i++) {
                ret.ws.At(i) /= input.GetHeight();
                ret.bs.At(i) /= input.GetHeight();
            }
            ReturnFromBenchmark(Expected<NeuralNetwork<T>>(ret));
        }
        /// @brief Learns
        /// @param diff Difference between expected and actual results
        /// @param rate Rate to learn in
        void Learn(const NeuralNetwork<T>& diff, T rate) {
            StartBenchmark
            for (size_t i = 0; i < count; i++) {
                ws.At(i) -= diff.ws.At(i) * rate;
                bs.At(i) -= diff.bs.At(i) * rate;
            }
            EndBenchmark
        }
        /// @brief Converts neural network to string
        /// @param padding String to pad with
        /// @return String representation of neural network
        virtual String ToString(const String& padding = "") const override {
            StartBenchmark
            String ret = padding + "a[0] = " + GetInput().ToString() + '\n';
            for (size_t i = 0; i < count; i++)
                ret += padding + "a[" + MathLib::ToString(i + 1) + "] = " + as.At(i + 1).ToString() + '\n' + padding + "b[" + MathLib::ToString(i) + "] = " + bs.At(i).ToString() + '\n' + padding + "w[" + MathLib::ToString(i) + "] = " + ws.At(i).ToString() + '\n';
            ReturnFromBenchmark(ret);
        }
        /// @brief Saves neural network data
        /// @param file File to save neural network data into
        /// @return Status
        virtual bool Save(Writeable& file) const override {
            StartBenchmark
            if (!file.Write<ActivationFunction>(activation) || !file.Write<size_t>(count) || !GetInput().Save(file)) ReturnFromBenchmark(false);
            for (size_t i = 0; i < count; i++)
                if (!as.At(i + 1).Save(file) || !ws.At(i).Save(file) || !bs.At(i).Save(file)) ReturnFromBenchmark(false);
            ReturnFromBenchmark(true);
        }
        /// @brief Loads neural network data
        /// @param file File to load neural network data from
        /// @return Status
        virtual bool Load(Readable& file) override {
            StartBenchmark
            if (!file.Read<ActivationFunction>(activation) || !file.Read<size_t>(count) || !as.Add(Matrix<T>()) || !GetInput().Load(file)) ReturnFromBenchmark(false);
            for (size_t i = 0; i < count; i++)
                if (!as.Add(Matrix<T>()) || !ws.Add(Matrix<T>()) || !bs.Add(Matrix<T>()) || !as.At(i + 1).Load(file) || !ws.At(i).Load(file) || !bs.At(i).Load(file)) ReturnFromBenchmark(false);
            ReturnFromBenchmark(true);
        }

        private:
        T Activation(const T& x) {
            StartBenchmark
            switch (activation) {
                case ActivationFunction::Sigmoid: ReturnFromBenchmark(Sigmoid<T>(x));
                case ActivationFunction::Tanh: ReturnFromBenchmark(HyperbolicTan<T>(x));
                case ActivationFunction::ReLU: ReturnFromBenchmark(x > 0 ? x : x);
                case ActivationFunction::LeakyReLU: ReturnFromBenchmark(x > 0 ? x : x * eps);
                default: ReturnFromBenchmark(MakeNaN());
            }
        }
        T ActivationDerivate(T y) {
            StartBenchmark
            switch (activation) {
                case ActivationFunction::Sigmoid: ReturnFromBenchmark(y * (1 - y));
                case ActivationFunction::Tanh: ReturnFromBenchmark(1 - y * y);
                case ActivationFunction::ReLU: ReturnFromBenchmark(y >= 0 ? 1 : 0);
                case ActivationFunction::LeakyReLU: ReturnFromBenchmark(y >= 0 ? 1 : eps);
                default: ReturnFromBenchmark(MakeNaN());
            }
        }

        /// @brief Count of weights, biases and count of neurons - 1
        size_t count;
        /// @brief Weights of neural network
        Array<Matrix<T>> ws;
        /// @brief Biases of neural network
        Array<Matrix<T>> bs;
        /// @brief Neurons of neural network
        Array<Matrix<T>> as;
    };
}

#endif