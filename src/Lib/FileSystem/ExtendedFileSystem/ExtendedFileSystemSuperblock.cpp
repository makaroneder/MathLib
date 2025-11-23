#include "ExtendedFileSystemSuperblock.hpp"

namespace MathLib {
    bool ExtendedFileSystemSuperblock::IsValid(void) const {
        return signature == expectedSignature;
    }
    uint32_t ExtendedFileSystemSuperblock::GetFirstFreeInode(void) const {
        return majorVersion < 1 ? 11 : firstFreeInode;
    }
    uint16_t ExtendedFileSystemSuperblock::GetInodeSize(void) const {
        return majorVersion < 1 ? 128 : inodeSize;
    }
    uint64_t ExtendedFileSystemSuperblock::GetBlockSize(void) const {
        return (uint64_t)1024 << blockSizeLog;
    }
    uint64_t ExtendedFileSystemSuperblock::GetFragmentSize(void) const {
        return (uint64_t)1024 << fragmentSizeLog;
    }
}