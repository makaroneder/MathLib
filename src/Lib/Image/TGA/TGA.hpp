#ifndef MathLib_Image_TGA_H
#define MathLib_Image_TGA_H
#include "../SaveableImage.hpp"

namespace MathLib {
    struct TGA : SaveableImage {
        /// @brief Creates a new TGA image
        /// @param width Width of the window
        /// @param height Height of the window
        TGA(size_t width, size_t height);
        /// @brief Saves data as TGA image
        /// @param file File to save data into
        /// @return Status
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        /// @brief Loads data from TGA image
        /// @param file File to load data from
        /// @return Status
        [[nodiscard]] virtual bool Load(Readable& file) override;
    };
}

#endif