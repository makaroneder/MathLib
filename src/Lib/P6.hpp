#ifndef P6_H
#define P6_H
#include "Image.hpp"

// TODO: Add support for P7, P3, P2, P5, P1, P4 and PF
struct P6 : SaveableImage {
    /// @brief Creates a new PPM P6 image
    /// @param width Width of the window
    /// @param height Height of the window
    P6(size_t width, size_t height);
    /// @brief Saves data as PPM P6 image
    /// @param file File to save data into
    /// @return Status
    virtual bool Save(Writeable& file) const override;
    /// @brief Loads data from PPM P6 image
    /// @param file File to load data from
    /// @return Status
    virtual bool Load(Readable& file) override;
};

#endif