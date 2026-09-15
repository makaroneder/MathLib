#include "Git.hpp"
#include <FileSystem/Directory.hpp>
#include <iostream>

void Main(int, char**, MathLib::FileSystem& fs) {
    // TODO: Packfiles https://git-scm.com/docs/pack-format
    MathLib::Directory gitDirectory = MathLib::Directory(fs, "Data/Git"_M);
    Git git = Git(gitDirectory, Git::ReferenceToCommit(gitDirectory, Git::GetDefaultReference(gitDirectory)));
    std::cout << git.ListFiles(""_M, 3) << std::endl;
}