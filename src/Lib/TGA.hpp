#ifndef TGA_H
#define TGA_H
#include "Image.hpp"

struct TGA : Image {
    /// @brief Creates a new TGA image
    /// @param w Width of the window
    /// @param h Height of the window
    TGA(const size_t& w, const size_t& h);
    /// @brief Saves data as TGA image
    /// @param file File to save data into
    /// @return Status
    virtual bool Save(Writeable& file) const override;
    /// @brief Loads data from TGA image
    /// @param file File to load data from
    /// @return Status
    virtual bool Load(Readable& file) override;
};

#endif