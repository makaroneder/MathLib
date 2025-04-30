#ifndef Action_H
#define Action_H
#include <stdint.h>

enum class Action : uint8_t {
    Reverse,
    BubbleSort,
    CocktailShakerSort,
    StoogeSort,
    OddEvenSort,
    InsertationSort,
};

#endif