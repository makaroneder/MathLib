#ifndef P6_H
#define P6_H
#include "Image.hpp"

struct P6 : Image {
    /// @brief Creates a new PPM P6 image
    /// @param w Width of the window
    /// @param h Height of the window
    P6(const size_t& w, const size_t& h);
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