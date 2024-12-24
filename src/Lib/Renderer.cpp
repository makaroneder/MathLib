#include "Renderer.hpp"

namespace MathLib {
    Renderer::Renderer(size_t width, size_t height) : SaveableImage(width, height), position(CreateVector<num_t>(0, 0, 0)), pointMultiplier(100), image(nullptr) {
        EmptyBenchmark
    }
    Renderer::~Renderer(void) {
        StartBenchmark
        if (image) delete image;
        EndBenchmark
    }
    Event Renderer::WaitForEvent(void) {
        StartBenchmark
        Event ret;
        while (ret.type == Event::Type::None) ret = GetEvent();
        ReturnFromBenchmark(ret);
    }
    bool Renderer::Save(Writeable& file) const {
        StartBenchmark
        if (!image) ReturnFromBenchmark(false);
        image->pixels = pixels;
        ReturnFromBenchmark(image->Save(file));
    }
    bool Renderer::Load(Readable& file) {
        StartBenchmark
        if (!image || !image->Load(file)) ReturnFromBenchmark(false);
        pixels = image->pixels;
        ReturnFromBenchmark(true);
    }
}