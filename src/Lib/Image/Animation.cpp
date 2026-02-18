#include "Animation.hpp"

namespace MathLib {
    Animation::Animation(void) : video(nullptr), timeLeft(0), frame(0) {}
    Animation::Animation(const Video* video) : video(video), timeLeft(video->AtUnsafe(0).duration), frame(0) {}
    bool Animation::IsEmpty(void) const {
        return !video || video->IsEmpty();
    }
    void Animation::Draw(Renderer& renderer, ssize_t centerX, ssize_t centerY) const {
        if (video) renderer.DrawImage(video->AtUnsafe(frame), centerX, centerY);
    }
    void Animation::Reset(void) {
        frame = 0;
        if (video) timeLeft = video->AtUnsafe(0).duration;
    }
    void Animation::Update(const num_t& value) {
        if (!video) return;
        const size_t frames = video->GetSize();
        timeLeft -= value;
        while (timeLeft <= 0) {
            frame = (frame + 1) % frames;
            timeLeft += video->AtUnsafe(frame).duration;
        }
    }
    void Animation::Reverse(const num_t& value) {
        if (!video) return;
        const size_t frames = video->GetSize();
        timeLeft += value;
        while (true) {
            const num_t prev = video->AtUnsafe(frame).duration;
            if (timeLeft <= prev) break;
            timeLeft -= prev;
            frame = frame ? (frame - 1) % frames : frames - 1;
        }
    }
    bool Animation::Equals(const Animation& other) const {
        return (!video || other.video) && (video || !other.video) && frame == other.frame && timeLeft == other.timeLeft && (video == other.video || *video == *other.video);
    }
}