#ifndef MathLib_Frame_H
#define MathLib_Frame_H
#include "Image/Image.hpp"
#include "Physics/SIUnits.hpp"

namespace MathLib {
    template <typename T>
    struct Frame : Image {
        Frame(void) : Image(0, 0), duration() {
            EmptyBenchmark
        }
        /// @brief Creates a new frame
        /// @param width Width of the frame
        /// @param height Height of the frame
        Frame(size_t width, size_t height, const Second<T>& duration) : Image(width, height), duration(duration) {
            EmptyBenchmark
        }
        Frame(const Image& image, const Second<T>& duration) : Image(image), duration(duration) {
            EmptyBenchmark
        }
        [[nodiscard]] bool IsValid(void) const {
            StartAndReturnFromBenchmark(duration.GetValue() >= 0);
        }
        [[nodiscard]] Second<T> GetDuration(void) const {
            StartAndReturnFromBenchmark(duration);
        }

        private:
        Second<T> duration;
    };
}

#endif