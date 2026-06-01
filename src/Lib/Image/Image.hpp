#ifndef MathLib_Image_H
#define MathLib_Image_H
#include "../Math/Matrix.hpp"

namespace MathLib {
    struct Image;
    struct Image : Comparable<Image> {
        Image(void);
        Image(size_t width, size_t height);
        [[nodiscard]] size_t GetWidth(void) const;
        [[nodiscard]] size_t GetHeight(void) const;
        [[nodiscard]] size_t GetSize(void) const;
        template <typename T>
        void FillBytes(T value);
        void Fill(uint32_t color);
        [[nodiscard]] uint32_t& AtUnsafe(size_t x, size_t y);
        [[nodiscard]] uint32_t AtUnsafe(size_t x, size_t y) const;
        [[nodiscard]] uint32_t& At(size_t x, size_t y);
        [[nodiscard]] uint32_t At(size_t x, size_t y) const;
        void CopyFromBuffer(const uint32_t* buffer);
        [[nodiscard]] bool CopyFromBuffer(const Collection<uint32_t>& buffer);
        [[nodiscard]] bool CopyFromBuffer(const Matrix<uint32_t>& buffer);
        [[nodiscard]] bool CopyFromBuffer(const Image& buffer);
        [[nodiscard]] Image Resize(size_t xMultiplier, size_t yMultiplier) const;
        [[nodiscard]] Image Mirror(void) const;
        [[nodiscard]] Image RotateUpsideDown(void) const;
        [[nodiscard]] Image SwapXY(void) const;
        [[nodiscard]] Image SwapXYAndRotateUpsideDown(void) const;
        void SetRectangle(ssize_t centerX, ssize_t centerY, size_t width, size_t height, uint32_t color);

        Matrix<uint32_t> pixels;

        protected:
        [[nodiscard]] virtual bool Equals(const Image& other) const override;
    };
}

#endif