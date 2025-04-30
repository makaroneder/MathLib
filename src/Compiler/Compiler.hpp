#ifndef Compiler_H
#define Compiler_H
#include "Toolchain/Evaluator.hpp"
#include "Variable.hpp"
#include "Type.hpp"
#include <Bitmap.hpp>

struct Compiler : Evaluator<MathLib::String> {
    enum class Register : uint8_t {
        A,
        B,
        C,
        D,
        SI,
        DI,
        R8,
        R9,
        R10,
        R11,
        R12,
        R13,
        R14,
        R15,
        Count,
    };
    Compiler(void);
    Compiler(Compiler* parent);
    [[nodiscard]] MathLib::String Evaluate(const Node& node);
    
    private:
    [[nodiscard]] size_t GetPushOffset(void) const;
    [[nodiscard]] MathLib::String RegisterToString(Register reg) const;
    [[nodiscard]] bool AllocRegister(Register& reg);
    [[nodiscard]] bool FreeRegister(Register reg);
    [[nodiscard]] size_t FindType(const MathLib::Sequence<char>& name) const;
    [[nodiscard]] Type GetType(size_t index) const;
    [[nodiscard]] MathLib::Expected<Variable> FindVariable(const MathLib::Sequence<char>& name) const;
    [[nodiscard]] MathLib::String LoadInto(const Node& node, Register& reg);
    [[nodiscard]] MathLib::String CompileCompound(const Node& node, Register& reg);

    static constexpr const char* returnInstruction = "jmp Return\n";

    MathLib::Bitmap registers;
    MathLib::Array<Variable> variables;
    MathLib::Array<Type> types;
    Compiler* parent;
    size_t pushOffset;
};

#endif