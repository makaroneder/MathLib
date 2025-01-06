#ifndef MathLib_EquationSolver_Preprocesor_H
#define MathLib_EquationSolver_Preprocesor_H
#include "../FileSystem/FileSystem.hpp"

namespace MathLib {
    /// @brief Reads file from given path
    /// @param path Path to the file to read
    /// @return Contents of the specified file
    [[nodiscard]] String Preproces(FileSystem& fileSystem, const String& path);
}

#endif