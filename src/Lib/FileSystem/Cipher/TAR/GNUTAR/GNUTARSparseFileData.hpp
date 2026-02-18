#ifndef MathLib_FileSystem_Cipher_TAR_GNU_SparseFileData_H
#define MathLib_FileSystem_Cipher_TAR_GNU_SparseFileData_H

namespace MathLib {
    struct GNUTARSparseFileData {
        char offset[12];
        char size[12];
    } __attribute__((packed));
}

#endif