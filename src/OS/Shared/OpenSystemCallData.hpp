#ifndef Shared_OpenSystemCallData_H
#define Shared_OpenSystemCallData_H

struct OpenSystemCallData {
    const char* path;
    bool create;

    OpenSystemCallData(const char* path, bool create);
};

#endif