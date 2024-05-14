#ifndef Test_H
#define Test_H
#include <Renderer.hpp>

struct Test : Renderer {
    /// @brief Creates a new renderer
    /// @param w Width of the window
    /// @param h Height of the window
    Test(size_t w, size_t h);
    /// @brief Updates renderer
    /// @return Status
    virtual bool Update(void) override;
    /// @brief Gets current event
    /// @return Event
    virtual Event GetEvent(void) override;

    size_t failed;
    std::vector<Matrix<num_t>> values;
};

#endif