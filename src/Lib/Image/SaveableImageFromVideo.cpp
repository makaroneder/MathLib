#include "SaveableImageFromVideo.hpp"

namespace MathLib {
    SaveableImageFromVideo::SaveableImageFromVideo(size_t width, size_t height, SaveableVideo* video, size_t frame) : SaveableImage(width, height), video(video), frame(frame) {
        if (video->IsEmpty()) {
            for (size_t i = 0; i < frame; i++)
                if (!video->Add(Frame(width, height, 0))) Panic("Failed to add frame to video");
            if (!video->Add(Frame(width, height, 1))) Panic("Failed to add frame to video");
        }
    }
    SaveableImageFromVideo::~SaveableImageFromVideo(void) {
        delete video;
    }
    void SaveableImageFromVideo::SwitchFrame(size_t newFrame, bool saveFrame, bool loadFrame) {
        if (saveFrame) video->At(frame).pixels = pixels;
        frame = newFrame;
        if (loadFrame) pixels = video->At(frame).pixels;
    }
    bool SaveableImageFromVideo::Save(Writable& file) const {
        video->At(frame).pixels = pixels;
        return video->Save(file);
    }
    bool SaveableImageFromVideo::Load(Readable& file) {
        if (!video->Load(file)) return false;
        pixels = video->At(frame).pixels;
        return true;
    }
}