#ifndef LittleAndBigEndian_H
#define LittleAndBigEndian_H

template <typename T>
struct LittleAndBigEndian {
    T little;
    T big;
} __attribute__((packed));

#endif