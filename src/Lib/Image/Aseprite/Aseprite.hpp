#ifndef MathLib_Image_Aseprite_H
#define MathLib_Image_Aseprite_H
#include "../SaveableImage.hpp"

namespace MathLib {
    struct Aseprite : SaveableImage {
        /// @brief Creates a new PPM P6 image
        /// @param width Width of the window
        /// @param height Height of the window
        Aseprite(size_t width, size_t height);
        /// @brief Saves data as PPM P6 image
        /// @param file File to save data into
        /// @return Status
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        /// @brief Loads data from PPM P6 image
        /// @param file File to load data from
        /// @return Status
        [[nodiscard]] virtual bool Load(Readable& file) override;
    };
}

#endif