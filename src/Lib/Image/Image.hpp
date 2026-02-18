#ifndef MathLib_Image_H
#define MathLib_Image_H
#include "../Math/Matrix.hpp"

namespace MathLib {
    struct Image;
    struct Image : Comparable<Image> {
        Image(void);
        /// @brief Creates a new image
        /// @param width Width of the image
        /// @param height Height of the image
        Image(size_t width, size_t height);
        /// @brief Returns width of the image
        /// @return Width of the image
        [[nodiscard]] size_t GetWidth(void) const;
        /// @brief Returns height of the image
        /// @return Height of the image
        [[nodiscard]] size_t GetHeight(void) const;
        [[nodiscard]] size_t GetSize(void) const;
        template <typename T>
        void FillBytes(T value);
        /// @brief Fills image
        /// @param color Color to fill the image with
        void Fill(uint32_t color);
        /// @brief Returns data at specified position
        /// @param x X position
        /// @param y Y position
        /// @return Data at specified position
        [[nodiscard]] uint32_t& AtUnsafe(size_t x, size_t y);
        /// @brief Returns data at specified position
        /// @param x X position
        /// @param y Y position
        /// @return Data at specified position
        [[nodiscard]] uint32_t AtUnsafe(size_t x, size_t y) const;
        /// @brief Returns data at specified position
        /// @param x X position
        /// @param y Y position
        /// @return Data at specified position
        [[nodiscard]] uint32_t& At(size_t x, size_t y);
        /// @brief Returns data at specified position
        /// @param x X position
        /// @param y Y position
        /// @return Data at specified position
        [[nodiscard]] uint32_t At(size_t x, size_t y) const;
        void CopyFromBuffer(const uint32_t* buffer);
        [[nodiscard]] bool CopyFromBuffer(const Collection<uint32_t>& buffer);
        [[nodiscard]] bool CopyFromBuffer(const Matrix<uint32_t>& buffer);
        [[nodiscard]] bool CopyFromBuffer(const Image& buffer);
        [[nodiscard]] Image Resize(size_t xMultiplier, size_t yMultiplier) const;
        [[nodiscard]] Image RotateUpsideDown(void) const;
        [[nodiscard]] Image SwapXY(void) const;
        [[nodiscard]] Image SwapXYAndRotateUpsideDown(void) const;

        /// @brief Pixels
        Matrix<uint32_t> pixels;

        protected:
        [[nodiscard]] virtual bool Equals(const Image& other) const override;
    };
}

#endif