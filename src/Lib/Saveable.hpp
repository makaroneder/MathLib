#ifndef Saveable_H
#define Saveable_H
#include <stdio.h>
#include <string>

struct Saveable {
    virtual ~Saveable(void);
    virtual bool Save(FILE* file) const = 0;
    virtual bool Load(FILE* file) = 0;
    bool SaveFromPath(std::string path) const;
    bool LoadFromPath(std::string path);
};

#endif