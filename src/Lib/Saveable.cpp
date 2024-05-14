#include "Saveable.hpp"

Saveable::~Saveable(void) {}
bool Saveable::SaveFromPath(std::string path) const {
    FILE* file = fopen(path.c_str(), "wb");
    return file != nullptr && Save(file) && !fflush(file) && !fclose(file);
}
bool Saveable::LoadFromPath(std::string path) {
    FILE* file = fopen(path.c_str(), "rb");
    return file != nullptr && Load(file) && !fflush(file) && !fclose(file);
}