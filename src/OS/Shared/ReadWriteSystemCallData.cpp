#include "ReadWriteSystemCallData.hpp"

ReadWriteSystemCallData::ReadWriteSystemCallData(uint64_t file, uint64_t address, uint64_t size, uint64_t position) : file(file), address(address), size(size), position(position) {}