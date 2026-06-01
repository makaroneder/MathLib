#include "SystemCall.hpp"
#include "Arch/Arch.hpp"
#include "VFS.hpp"
#include <Interfaces/Sequence/ExternString.hpp>
#include <ReadWriteSystemCall.h>
#include <OpenSystemCall.h>

uint64_t OnSystemCall(SystemCall num, void* args) {
    switch (num) {
        case SystemCall::ExitSystemCall: return ArchRemoveTask(*(const uint64_t*)args);
        case SystemCall::GetTaskSystemCall: return ArchGetCurrentTask();
        case SystemCall::AllocateMemorySystemCall: return (uint64_t)new uint8_t [*(const uint64_t*)args];
        case SystemCall::FreeMemorySystemCall: {
            delete [] (uint8_t*)args;
            return 0;
        }
        case SystemCall::ReadFileSystemCall: {
            const ReadWriteSystemCall* const data = (const ReadWriteSystemCall*)args;
            return vfs.Read(data->file, (void*)data->address, data->size, data->position);
        }
        case SystemCall::WriteFileSystemCall: {
            const ReadWriteSystemCall* const data = (const ReadWriteSystemCall*)args;
            return vfs.Write(data->file, (const void*)data->address, data->size, data->position);
        }
        case SystemCall::GetSizeOfFileSystemCall: return vfs.GetSize(*(const uint64_t*)args);
        case SystemCall::CloseFileSystemCall: return vfs.Close(*(const uint64_t*)args);
        case SystemCall::OpenFileSystemCall: {
            const OpenSystemCall* const data = (const OpenSystemCall*)args;
            return vfs.OpenInternal(MathLib::ExternString((char*)data->path), data->create ? MathLib::OpenMode::Write : MathLib::OpenMode::ReadWrite);
        }
        default: return UINT64_MAX;
    }
}