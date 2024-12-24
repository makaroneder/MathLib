#include "Aseprite.hpp"

namespace MathLib {
    Aseprite::Aseprite(size_t width, size_t height) : SaveableImage(width, height) {
        EmptyBenchmark
    }
    bool Aseprite::Save(Writeable& file) const {
        // TODO:
        (void)file;
        StartBenchmark
        ReturnFromBenchmark(false);
    }
    bool Aseprite::Load(Readable& file) {
        // TODO:
        (void)file;
        StartBenchmark
        ReturnFromBenchmark(false);
    }
}