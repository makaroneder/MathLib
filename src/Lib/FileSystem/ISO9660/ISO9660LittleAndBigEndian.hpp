#ifndef MathLib_FileSystem_ISO9660_LittleAndBigEndian_H
#define MathLib_FileSystem_ISO9660_LittleAndBigEndian_H

namespace MathLib {
    template <typename T>
    struct ISO9660LittleAndBigEndian {
        T little;
        T big;

        ISO9660LittleAndBigEndian(void) : little(), big() {}
    } __attribute__((packed));
}

#endif