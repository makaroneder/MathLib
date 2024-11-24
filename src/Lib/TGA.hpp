#ifndef TGA_H
#define TGA_H
#include "Image.hpp"

namespace MathLib {
    struct TGA : SaveableImage {
        /// @brief Creates a new TGA image
        /// @param width Width of the window
        /// @param height Height of the window
        TGA(size_t width, size_t height);
        /// @brief Saves data as TGA image
        /// @param file File to save data into
        /// @return Status
        virtual bool Save(Writeable& file) const override;
        /// @brief Loads data from TGA image
        /// @param file File to load data from
        /// @return Status
        virtual bool Load(Readable& file) override;
    };
}

#endif