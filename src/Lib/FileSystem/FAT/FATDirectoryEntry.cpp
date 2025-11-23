#include "FATDirectoryEntry.hpp"
#include "../../Emulator/Register.hpp"
#include "../../Memory.hpp"

namespace MathLib {
    FATDirectoryEntry::FATDirectoryEntry(void) : readOnly(false), hidden(false), system(false), volumeID(false), directory(false), archive(false), reserved(0), creationTime(0), time(0), lastAccessedDate(0), highCluster(0), lastModificationTime(0), lowCluster(0), size(0) {}
    bool FATDirectoryEntry::IsLongFileName(void) const {
        return readOnly && hidden && system && volumeID && directory && archive;
    }
    String FATDirectoryEntry::GetName(void) const {
        String n;
        for (size_t i = 0; i < 8; i++)
            if (name[i] != ' ') n += name[i];
        String ext;
        for (size_t i = 8; i < 11; i++)
            if (name[i] != ' ') ext += name[i];
        return n + (ext.IsEmpty() ? "" : ('.'_M + ext));
    }
    uint32_t FATDirectoryEntry::GetCluster(void) const {
        Register ret;
        ret.Set16(lowCluster, false);
        ret.Set16(highCluster, true);
        return ret.Get32(false);
    }
    void FATDirectoryEntry::SetCluster(uint32_t cluster) {
        const Register ret = Register(cluster);
        lowCluster = ret.Get16(false);
        highCluster = ret.Get16(true);
    }
    bool FATDirectoryEntry::operator==(const FATDirectoryEntry& other) const {
        if (readOnly != other.readOnly || hidden != other.hidden || system != other.size || volumeID != other.volumeID || directory != other.directory || archive != other.archive || reserved != other.reserved || creationTime != other.creationTime || time != other.time || lastAccessedDate != other.lastAccessedDate || highCluster != other.highCluster || lastModificationTime != other.lastModificationTime || lowCluster != other.lowCluster || size != other.size) return false;
        for (uint8_t i = 0; i < SizeOfArray(name); i++)
            if (name[i] != other.name[i]) return false;
        return true;
    }
    bool FATDirectoryEntry::operator!=(const FATDirectoryEntry& other) const {
        return !(*this == other);
    }
}