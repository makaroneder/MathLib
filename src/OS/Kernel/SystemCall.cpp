#include "SystemCall.hpp"
#include "Arch/Arch.hpp"
#include "VFS.hpp"
#include <Interfaces/Sequence/ExternString.hpp>
#include <ReadWriteSystemCallData.hpp>
#include <OpenSystemCallData.hpp>

uint64_t OnSystemCall(SystemCall systemCall, void* args) {
    switch (systemCall) {
        case SystemCall::Exit: return ArchRemoveTask(*(const uint64_t*)args);
        case SystemCall::GetTask: return ArchGetCurrentTask();
        case SystemCall::AllocateMemory: return (uint64_t)new uint8_t [*(const uint64_t*)args];
        case SystemCall::FreeMemory: {
            delete [] (uint8_t*)args;
            return 0;
        }
        case SystemCall::ReadFile: {
            const ReadWriteSystemCallData* const data = (const ReadWriteSystemCallData*)args;
            return vfs.Read(data->file, (void*)data->address, data->size, data->position);
        }
        case SystemCall::WriteFile: {
            const ReadWriteSystemCallData* const data = (const ReadWriteSystemCallData*)args;
            return vfs.Write(data->file, (const void*)data->address, data->size, data->position);
        }
        case SystemCall::GetSizeOfFile: return vfs.GetSize(*(const uint64_t*)args);
        case SystemCall::CloseFile: return vfs.Close(*(const uint64_t*)args);
        case SystemCall::OpenFile: {
            const OpenSystemCallData* const data = (const OpenSystemCallData*)args;
            return vfs.OpenInternal(MathLib::ExternString((char*)data->path), data->create ? MathLib::OpenMode::Write : MathLib::OpenMode::ReadWrite);
        }
        default: return UINT64_MAX;
    }
}