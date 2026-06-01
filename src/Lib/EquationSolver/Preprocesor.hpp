#ifndef MathLib_EquationSolver_Preprocesor_H
#define MathLib_EquationSolver_Preprocesor_H
#include "../FileSystem/FileSystem.hpp"

namespace MathLib {
    [[nodiscard]] String Preproces(FileSystem& fileSystem, const Sequence<char>& path);
}

#endif