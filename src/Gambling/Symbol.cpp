#include "Symbol.hpp"

Symbol::Symbol(std::string path, std::string n, std::vector<size_t> mult) : WavefrontObject<num_t>(CreateVector<num_t>(0, 0, 0), path), name(n), multiplier(mult) {}
bool Symbol::operator==(const Symbol& other) const {
    return name == other.name;
}