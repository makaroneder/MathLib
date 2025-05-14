#ifndef Compiler_H
#define Compiler_H
#include "Toolchain/Evaluator.hpp"
#include "Variable.hpp"
#include "Type.hpp"
#include <Bitmap.hpp>

struct Compiler : Evaluator<MathLib::String> {
    Compiler(void);
    Compiler(Compiler* parent);
    [[nodiscard]] MathLib::String Evaluate(const Node& node);
    
    private:
    [[nodiscard]] size_t FindType(const MathLib::Sequence<char>& name) const;
    [[nodiscard]] Type GetType(size_t index) const;
    [[nodiscard]] MathLib::Expected<Variable> FindVariable(const MathLib::Sequence<char>& name) const;
    [[nodiscard]] MathLib::String LoadInto(const Node& node);
    [[nodiscard]] MathLib::String CompileCompound(const Node& node);
    [[nodiscard]] MathLib::String AllocStack(ssize_t size);

    static constexpr const char* returnInstruction = "jmp Return\n";
    static constexpr size_t wordSize = 8;

    MathLib::Array<Variable> variables;
    MathLib::Array<Type> types;
    Compiler* parent;
    size_t pushOffset;
};

#endif