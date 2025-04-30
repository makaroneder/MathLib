#ifndef FileCipherData_H
#define FileCipherData_H
#include <Typedefs.hpp>

struct FileCipherData {
    enum class Type {
        Invalid,
        Normal,
        Skip,
    };
    MathLib::String name;
    size_t size;
    Type type;

    FileCipherData(void);
    FileCipherData(Type type, const MathLib::String& name, size_t size);
};

#endif