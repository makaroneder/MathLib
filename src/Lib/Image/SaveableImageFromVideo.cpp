#include "SaveableImageFromVideo.hpp"

namespace MathLib {
    SaveableImageFromVideo::SaveableImageFromVideo(size_t width, size_t height, SaveableVideo* video, size_t frame) : SaveableImage(width, height), video(video), frame(frame) {
        StartBenchmark
        if (video->IsEmpty()) {
            for (size_t i = 0; i < frame; i++)
                if (!video->Add(Frame(width, height, 0))) Panic("Failed to add frame to video");
            if (!video->Add(Frame(width, height, 1))) Panic("Failed to add frame to video");
        }
        EndBenchmark
    }
    SaveableImageFromVideo::~SaveableImageFromVideo(void) {
        StartBenchmark
        delete video;
        EndBenchmark
    }
    void SaveableImageFromVideo::SwitchFrame(size_t newFrame, bool saveFrame, bool loadFrame) {
        StartBenchmark
        if (saveFrame) video->At(frame).pixels = pixels;
        frame = newFrame;
        if (loadFrame) pixels = video->At(frame).pixels;
        EndBenchmark
    }
    bool SaveableImageFromVideo::Save(Writable& file) const {
        StartBenchmark
        video->At(frame).pixels = pixels;
        ReturnFromBenchmark(video->Save(file));
    }
    bool SaveableImageFromVideo::Load(Readable& file) {
        StartBenchmark
        if (!video->Load(file)) ReturnFromBenchmark(false);
        pixels = video->At(frame).pixels;
        ReturnFromBenchmark(true);
    }
}