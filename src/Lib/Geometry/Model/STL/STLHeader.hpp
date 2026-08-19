#ifndef MathLib_Geometry_Model_STL_Header_H
#define MathLib_Geometry_Model_STL_Header_H
#include <stdint.h>

namespace MathLib {
    struct STLHeader {
        uint64_t reserved[10];
        uint32_t triangleCount;
        
        STLHeader(uint32_t triangleCount = 0);
    } __attribute__((packed));
}
    
#endif