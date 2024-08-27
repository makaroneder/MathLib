#include "VFSFile.hpp"

VFSFile::VFSFile(size_t fs, size_t index) : fs(fs), index(index), free(false) {}