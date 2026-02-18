#ifndef MathLib_Image_Animation_H
#define MathLib_Image_Animation_H
#include "Video.hpp"
#include "../Renderer.hpp"

namespace MathLib {
    struct Animation;
    struct Animation : Comparable<Animation> {
        Animation(void);
        Animation(const Video* video);
        [[nodiscard]] bool IsEmpty(void) const;
        void Draw(Renderer& renderer, ssize_t centerX, ssize_t centerY) const;
        void Reset(void);
        void Update(const num_t& value);
        void Reverse(const num_t& value);

        protected:
        [[nodiscard]] virtual bool Equals(const Animation& other) const override;

        private:
        const Video* video;
        num_t timeLeft;
        size_t frame;
    };
}

#endif