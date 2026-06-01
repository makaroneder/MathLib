#include "Git.hpp"
#include <FileSystem/Directory.hpp>
#include <Libc/HostFileSystem.hpp>
#include <iostream>

int main(int, char**) {
    try {
        // TODO: Packfiles https://git-scm.com/docs/pack-format
        MathLib::HostFileSystem fs;
        MathLib::Directory gitDirectory = MathLib::Directory(fs, "src/TestPrograms/Git"_M);
        Git git = Git(gitDirectory, Git::ReferenceToCommit(gitDirectory, Git::GetDefaultReference(gitDirectory)));
        std::cout << git.ListFiles(""_M, 3) << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}