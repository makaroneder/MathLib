#ifndef FileSystems_ISO9660_LittleAndBigEndian_H
#define FileSystems_ISO9660_LittleAndBigEndian_H

namespace MathLib {
    template <typename T>
    struct ISO9660LittleAndBigEndian {
        T little;
        T big;
    } __attribute__((packed));
}

#endif