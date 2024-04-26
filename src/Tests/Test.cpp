#include "Test.hpp"

Test::Test(size_t w, size_t h) : Renderer(w, h) {
    failed = 0;
    values = {};
}
bool Test::UpdateInternal(Matrix<uint32_t> pixels) {
    for (size_t y = 0; y < pixels.GetHeight(); y++) {
        for (size_t x = 0; x < pixels.GetWidth(); x++) {
            const bool expected = (pixels.At(x, y) != 0);
            bool found = expected;
            for (size_t i = 0; i < values.size() && (found == expected); i++)
                if (values[i] == CreateVector<num_t>(x, y, 0)) found = !expected;
            if (found == expected) failed++;
        }
    }
    return true;
}
Event Test::GetEvent(void) {
    return Event();
}