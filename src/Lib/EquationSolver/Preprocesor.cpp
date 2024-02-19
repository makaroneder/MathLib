#include "Preprocesor.hpp"
#include <fstream>

std::string Preproces(std::string path) {
    std::ifstream file;
    file.open(path);
    std::string ret = "";
    if (file.is_open()) {
        while (file) {
            std::string line;
            std::getline(file, line);
            ret += line + "\n";
        }
    }
    ret.pop_back();
    return ret;
}