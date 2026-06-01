#ifndef SystemCalls_H
#define SystemCalls_H

enum SystemCall {
    ExitSystemCall = 0,
    GetTaskSystemCall,
    AllocateMemorySystemCall,
    FreeMemorySystemCall,
    ReadFileSystemCall,
    WriteFileSystemCall,
    GetSizeOfFileSystemCall,
    CloseFileSystemCall,
    OpenFileSystemCall,
};

#endif