#ifndef GNUTARSparseFileData_H
#define GNUTARSparseFileData_H

struct GNUTARSparseFileData {
    char offset[12];
    char size[12];
} __attribute__((packed));

#endif