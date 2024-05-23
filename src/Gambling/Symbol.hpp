#ifndef Symbol_H
#define Symbol_H
#include <Geometry/WavefrontObject.hpp>

struct Symbol : WavefrontObject<num_t> {
    std::string name;
    std::vector<size_t> multiplier;

    Symbol(std::string path, std::string n, std::vector<size_t> mult);
    bool operator==(const Symbol& other) const;
};

#endif