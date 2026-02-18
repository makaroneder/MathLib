#include "Box.hpp"

Box::Box(void) : materials(materialCount), machine() {
    materials.Fill(false);
}
bool Box::ContainsMaterial(size_t material) const {
    return material < materialCount && materials.AtUnsafe(material);
}
bool Box::Equals(const Box& other) const {
    return materials == other.materials && machine == other.machine;
}