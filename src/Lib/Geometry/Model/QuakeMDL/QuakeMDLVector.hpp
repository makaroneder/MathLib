#ifndef MathLib_Geometry_Model_QuakeMDL_Vector_H
#define MathLib_Geometry_Model_QuakeMDL_Vector_H

namespace MathLib {
    struct QuakeMDLVector {
        float x;
        float y;
        float z;
    } __attribute__((packed));
}

#endif