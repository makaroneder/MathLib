#include "../Libc/stdlib.h"
#include "../Libc/stdio.h"
#include "MainData.h"

bool constructorsCalled = false;
[[gnu::constructor]] void TestConstructors(void) {
    constructorsCalled = true;
}
extern "C" void _init(void);
extern "C" int main(int argc, char** argv);
extern "C" void _start(const MainData* data) {
    _init();
    if (!constructorsCalled) abort();
    if (data->out != UINT64_MAX) {
        stdout = stderr = (FILE*)malloc(sizeof(FILE));
        if (!stdout) abort();
        stdout->index = data->out;
        stdout->position = 0;
    }
    else stdout = stderr = NULL;
    if (data->in != UINT64_MAX) {
        stdin = (FILE*)malloc(sizeof(FILE));
        if (!stdin) abort();
        stdin->index = data->in;
        stdin->position = 0;
    }
    else stdin = NULL;
    exit(main(data->argc, data->argv));
}