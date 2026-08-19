#include "SaveableImageFromVideo.hpp"

namespace MathLib {
    SaveableImageFromVideo::SaveableImageFromVideo(void) : SaveableImage(), video(nullptr), frame(SIZE_MAX) {}
    SaveableImageFromVideo::SaveableImageFromVideo(size_t width, size_t height, SaveableVideo* video, size_t frame) : SaveableImage(width, height), video(video), frame(frame) {
        if (!(video && video->IsEmpty())) return;
        for (size_t i = 0; i < frame; i++)
            if (!video->Add(Frame(width, height, 0))) Panic("Failed to add frame to video");
        if (!video->Add(Frame(width, height, 1))) Panic("Failed to add frame to video");
    }
    SaveableImageFromVideo::~SaveableImageFromVideo(void) {
        if (video) delete video;
    }
    void SaveableImageFromVideo::SwitchFrame(size_t newFrame, bool saveFrame, bool loadFrame) {
        if (!video) return;
        if (saveFrame) video->At(frame).pixels = pixels;
        frame = newFrame;
        if (loadFrame) pixels = video->At(frame).pixels;
    }
    bool SaveableImageFromVideo::Save(Writable& file) const {
        if (!video) return false;
        video->At(frame).pixels = pixels;
        return video->Save(file);
    }
    bool SaveableImageFromVideo::Load(Readable& file) {
        if (!(video && video->Load(file))) return false;
        pixels = video->At(frame).pixels;
        return true;
    }
}