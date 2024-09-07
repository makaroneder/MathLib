#include "MemoryFSFile.hpp"

MemoryFSFile::MemoryFSFile(const size_t& resource, const bool& write) : resource(resource), write(write), free(false) {}