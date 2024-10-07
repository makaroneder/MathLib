#include "MemoryFSFile.hpp"

MemoryFSFile::MemoryFSFile(size_t resource, bool write) : resource(resource), write(write), free(false) {}