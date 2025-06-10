#ifndef Compiler_H
#define Compiler_H
#include "Variable.hpp"
#include "Type.hpp"
#include <Bitmap.hpp>
#include <Compiler/Evaluator.hpp>

struct Compiler : MathLib::Evaluator<MathLib::String> {
    Compiler(void);
    Compiler(Compiler* parent);
    [[nodiscard]] MathLib::String Evaluate(const MathLib::ParserNode& node);
    
    private:
    [[nodiscard]] size_t FindType(const MathLib::Sequence<char>& name) const;
    [[nodiscard]] Type GetType(size_t index) const;
    [[nodiscard]] MathLib::Expected<Variable> FindVariable(const MathLib::Sequence<char>& name) const;
    [[nodiscard]] MathLib::String LoadInto(const MathLib::ParserNode& node);
    [[nodiscard]] MathLib::String CompileCompound(const MathLib::ParserNode& node);
    [[nodiscard]] MathLib::String AllocStack(ssize_t size);

    static constexpr const char* returnInstruction = "jmp Return\n";
    static constexpr size_t wordSize = 8;

    MathLib::Array<Variable> variables;
    MathLib::Array<Type> types;
    Compiler* parent;
    size_t pushOffset;
};

#endif