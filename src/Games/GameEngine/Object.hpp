#ifndef Object_H
#define Object_H
#include <EquationSolver/Optimizer.hpp>
#include <FileSystem/FileSystem.hpp>
#include <JSON.hpp>

struct Object {
    Object(void);
    Object(MathLib::Optimizer& baseOptimizer, MathLib::FileSystem& fileSystem, const MathLib::String& path, const MathLib::JSON& json);
    MathLib::String GetName(void) const;
    MathLib::Node* Call(const MathLib::String& function, const MathLib::String& arg) const;
    void Destroy(void);

    private:
    MathLib::String name;
    MathLib::Optimizer* optimizer;
};

#endif