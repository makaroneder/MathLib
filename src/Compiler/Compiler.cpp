#include "Compiler.hpp"
#include "NodeType.hpp"
#include <String.hpp>

// TODO: Mutable variables, types, objects

Compiler::Compiler(void) : registers((size_t)Register::Count), variables(), types(), parent(nullptr), pushOffset(0) {}
Compiler::Compiler(Compiler* parent) : registers(), variables(), types(), parent(parent), pushOffset(0) {}
size_t Compiler::GetPushOffset(void) const {
    return pushOffset + (parent ? parent->GetPushOffset() : 0);
}
MathLib::String Compiler::RegisterToString(Register reg) const {
    switch (reg) {
        case Register::A: return "rax";
        case Register::B: return "rbx";
        case Register::C: return "rcx";
        case Register::D: return "rdx";
        case Register::SI: return "rsi";
        case Register::DI: return "rdi";
        case Register::R8: return "r8";
        case Register::R9: return "r9";
        case Register::R10: return "r10";
        case Register::R11: return "r11";
        case Register::R12: return "r12";
        case Register::R13: return "r13";
        case Register::R14: return "r14";
        case Register::R15: return "r15";
        default: return "";
    }
}
bool Compiler::AllocRegister(Register& reg) {
    if (parent) return parent->AllocRegister(reg);
    if ((size_t)reg < (size_t)Register::Count && registers.At((size_t)reg))
        return registers.Set((size_t)reg, false);
    for (size_t i = 0; i < (size_t)Register::Count; i++) {
        if (registers.At(i)) {
            reg = (Register)i;
            return registers.Set(i, false);
        }
    }
    return false;
}
bool Compiler::FreeRegister(Register reg) {
    return parent ? parent->FreeRegister(reg) : registers.Set((size_t)reg, true);
}
size_t Compiler::FindType(const MathLib::Sequence<char>& name) const {
    if (parent) return parent->FindType(name);
    const size_t size = types.GetSize();
    for (size_t i = 0; i < size; i++)
        if (types.At(i).name == name) return i;
    return SIZE_MAX;
}
Type Compiler::GetType(size_t index) const {
    return parent ? parent->GetType(index) : types.At(index);
}
MathLib::Expected<Variable> Compiler::FindVariable(const MathLib::Sequence<char>& name) const {
    for (const Variable& variable : variables)
        if (variable.name == name) return variable;
    return parent ? parent->FindVariable(name) : MathLib::Expected<Variable>();
}
MathLib::String Compiler::CompileCompound(const Node& node, Register& reg) {
    if (node.GetType() != (size_t)NodeType::Comma) return LoadInto(node, reg);
    const size_t size = node.GetSize();
    MathLib::String ret;
    for (size_t i = 0; i < size; i++) {
        if (i && !FreeRegister(reg)) return "";
        ret += LoadInto(node.At(i), reg);
    }
    return ret;
}
MathLib::String Compiler::LoadInto(const Node& node, Register& reg) {
    switch ((NodeType)node.GetType()) {
        case NodeType::Return: return LoadInto(node.At(0), reg) + returnInstruction;
        case NodeType::Digit: return AllocRegister(reg) ? "mov "_M + RegisterToString(reg) + ", " + node.GetData() + '\n' : "";
        case NodeType::Identifier: {
            const MathLib::Expected<Variable> variable = FindVariable(node.GetData());
            if (!variable.HasValue() || !AllocRegister(reg)) return "";
            const MathLib::String ret = "mov "_M + RegisterToString(reg) + ", [rbp";
            const size_t offset = variable.Get().offset;
            if (!offset) return ret + "]\n";
            return ret + " + " + MathLib::ToString(offset, 10) + "]\n";
        }
        case NodeType::Addition: {
            Register other = Register::Count;
            MathLib::String ret = LoadInto(node.At(0), reg) + LoadInto(node.At(1), other);
            if (!FreeRegister(other)) return "";
            const MathLib::String operation = node.GetData();
            if (operation == "+") ret += "add";
            else if (operation == "-") ret += "sub";
            else return "";
            return ret + ' ' + RegisterToString(reg) + ", " + RegisterToString(other) + '\n';
        }
        case NodeType::Multiplication: {
            Register other = Register::Count;
            MathLib::String ret = LoadInto(node.At(0), reg) + LoadInto(node.At(1), other);
            if (reg != Register::A) ret += "push rax\n";
            if (reg != Register::D) ret += "push rdx\n";
            ret += "mov [buffer], "_M + RegisterToString(other) + "\nmov rax, "_M + RegisterToString(reg) + '\n';
            const MathLib::String operation = node.GetData();
            if (operation == "*") ret += "mul";
            else if (operation == "/") ret += "div";
            else return "";
            ret += " qword [buffer]\n";
            if (reg != Register::D) ret += "pop rdx\n";
            if (reg != Register::A) {
                ret += "mov [buffer], rax\n";
                ret += "pop rax\n";
                ret += "mov "_M + RegisterToString(reg) + ", [buffer]\n";
            }
            return ret;
        }
        case NodeType::VariableDefinition: {
            const Node type = node.At(0);
            if (type.GetType() != (size_t)NodeType::Identifier) return "";
            const size_t typeIndex = FindType(type.GetData());
            if (typeIndex == SIZE_MAX) return "";
            const size_t size = GetType(typeIndex).size;
            if (!variables.Add(Variable(node.GetData(), typeIndex, GetPushOffset()))) return "";
            pushOffset += size;
            return "add rsp, "_M + MathLib::ToString(size, 10) + '\n';
        }
        case NodeType::FunctionDefinition: {
            // TODO: Load arguments
            const MathLib::String name = node.GetData();
            Compiler function = Compiler(this);
            const MathLib::String ret = "global "_M + name + '\n' + name + ":\npush rbp\nmov rbp, rsp\n" + function.CompileCompound(node.At(1), reg);
            return ret + function.CompileCompound(node.At(2), reg) + returnInstruction;
        }
        default: return "";
    }
}
MathLib::String Compiler::Evaluate(const Node& node) {
    if (parent || !variables.Reset() || !types.Reset() || !types.Add(Type("uint64"_M, sizeof(uint64_t)))) return "";
    pushOffset = 0;
    registers.Fill(true);
    Register reg = Register::DI;
    MathLib::String ret = "bits 64\nsection .data\nbuffer: dq 0\nsection .text\nReturn:\nmov rsp, rbp\npop rbp\nret\nglobal _start\n_start:\ncall Main\n"_M;
    const MathLib::String tmp = LoadInto(node, reg);
    if (reg != Register::DI) ret += "mov rdi, "_M + RegisterToString(reg) + '\n';
    return ret + "mov rax, 60\nsyscall\n" + tmp;
}