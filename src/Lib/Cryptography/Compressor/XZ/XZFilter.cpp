#include "XZFilter.hpp"

namespace MathLib {
    XZFilter::XZFilter(void) : data(), id() {}
    XZFilter::XZFilter(Type id, const Array<uint8_t>& data) : data(data), id(id) {}
    bool XZFilter::Equals(const XZFilter& other) const {
        return id == other.id && data == other.data;
    }
}