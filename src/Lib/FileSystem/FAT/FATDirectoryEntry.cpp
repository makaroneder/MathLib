#include "FATDirectoryEntry.hpp"
#include "../../Emulator/Register.hpp"
#include "../../Memory.hpp"

namespace MathLib {
    bool FATDirectoryEntry::IsLongFileName(void) const {
        StartBenchmark
        ReturnFromBenchmark(readOnly && hidden && system && volumeID && directory && archive);
    }
    String FATDirectoryEntry::GetName(void) const {
        StartBenchmark
        String n;
        for (size_t i = 0; i < 8; i++)
            if (name[i] != ' ') n += name[i];
        String ext;
        for (size_t i = 8; i < 11; i++)
            if (name[i] != ' ') ext += name[i];
        ReturnFromBenchmark(n + (ext.IsEmpty() ? "" : ('.'_M + ext)));
    }
    uint32_t FATDirectoryEntry::GetCluster(void) const {
        StartBenchmark
        Register ret;
        ret.Set16(lowCluster, false);
        ret.Set16(highCluster, true);
        ReturnFromBenchmark(ret.Get32(false));
    }
    void FATDirectoryEntry::SetCluster(uint32_t cluster) {
        StartBenchmark
        const Register ret = Register(cluster);
        lowCluster = ret.Get16(false);
        highCluster = ret.Get16(true);
        EndBenchmark
    }
}