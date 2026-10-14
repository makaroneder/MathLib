#include "Saveable.hpp"
#include "FileSystem/File.hpp"
#include "Interfaces/Sequence/ByteArray.hpp"
#include "Interfaces/Sequence/SequenceReadable.hpp"

namespace MathLib {
    bool Saveable::SaveFromPath(FileSystem& fileSystem, const Sequence<char>& path) const {
        File file = fileSystem.Open(path, OpenMode::Write);
        return file.Exists() && Save(file);
    }
    bool Saveable::SaveFromPath(FileSystem* fileSystem, const Sequence<char>& path) const {
        return fileSystem && SaveFromPath(*fileSystem, path);
    }
    bool Saveable::SaveFromDatabase(Database& database, const Sequence<char>& path) const {
        ByteArray data;
        return Save(data) && database.CreateRecord(path, data);
    }
    bool Saveable::SaveFromDatabase(Database* database, const Sequence<char>& path) const {
        return database && SaveFromDatabase(*database, path);
    }
    bool Saveable::LoadFromPath(FileSystem& fileSystem, const Sequence<char>& path) {
        File file = fileSystem.Open(path, OpenMode::Read);
        return file.Exists() && Load(file);
    }
    bool Saveable::LoadFromPath(FileSystem* fileSystem, const Sequence<char>& path) {
        return fileSystem && LoadFromPath(*fileSystem, path);
    }
    bool Saveable::LoadFromDatabase(Database& database, const Sequence<char>& path) {
        ByteArray array = database.Read(path);
        return Load(array);
    }
    bool Saveable::LoadFromDatabase(Database* database, const Sequence<char>& path) {
        return database && LoadFromDatabase(*database, path);
    }
    bool Saveable::LoadFromSequence(const Sequence<uint8_t>& sequence) {
        SequenceReadable readable = sequence;
        return Load(readable);
    }
}