#include "Compiler.hpp"
#include "NodeType.hpp"
#include <String.hpp>

// TODO: Function calls, mutable variables, types, objects

Compiler::Compiler(void) : variables(), types(), parent(nullptr), pushOffset(0) {}
Compiler::Compiler(Compiler* parent) : variables(), types(), parent(parent), pushOffset(0) {}
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
MathLib::String Compiler::CompileCompound(const Node& node) {
    if (node.GetType() != (size_t)NodeType::Comma) return LoadInto(node);
    const size_t size = node.GetSize();
    MathLib::String ret;
    for (size_t i = 0; i < size; i++) ret += LoadInto(node.At(i));
    return ret;
}
MathLib::String Compiler::AllocStack(ssize_t size) {
    pushOffset += size;
    return (size < 0 ? "add"_M : "sub") + " rsp, "_M + MathLib::ToString(MathLib::Abs(size), 10) + '\n';
}
MathLib::String Compiler::LoadInto(const Node& node) {
    switch ((NodeType)node.GetType()) {
        case NodeType::Return: return LoadInto(node.At(0)) + returnInstruction;
        case NodeType::Digit: return "mov qword [rsp], "_M + node.GetData() + '\n' + AllocStack(wordSize);
        case NodeType::Identifier: {
            const MathLib::Expected<Variable> variable = FindVariable(node.GetData());
            if (!variable.HasValue()) return "";
            const size_t size = GetType(variable.Get().type).size;
            return "mov rsi, rbp - "_M + MathLib::ToString(variable.Get().offset, 10) + "\nmov rdi, rsp\nmov rcx, " + MathLib::ToString(size, 10) + "\nrep movsb\n" + AllocStack(size);
        }
        case NodeType::Addition: {
            MathLib::String ret = LoadInto(node.At(0)) + LoadInto(node.At(1)) + "mov rax, [rsp + 16]\nmov rbx, [rsp + 8]\n";
            const MathLib::String operation = node.GetData();
            if (operation == "+") ret += "add";
            else if (operation == "-") ret += "sub";
            else return "";
            return ret + " rax, rbx\nmov [rsp + 16], rax\n" + AllocStack(-wordSize);
        }
        case NodeType::Multiplication: {
            MathLib::String ret = LoadInto(node.At(0)) + LoadInto(node.At(1)) + "mov rax, [rsp + 16]\nmov rbx, [rsp + 8]\n";
            const MathLib::String operation = node.GetData();
            if (operation == "*") ret += "mul";
            else if (operation == "/") ret += "div";
            else return "";
            return ret + " rbx\nmov [rsp + 16], rax\n" + AllocStack(-wordSize);
        }
        case NodeType::VariableDefinition: {
            const Node type = node.At(0);
            if (type.GetType() != (size_t)NodeType::Identifier) return "";
            const size_t typeIndex = FindType(type.GetData());
            if (typeIndex == SIZE_MAX) return "";
            const size_t size = GetType(typeIndex).size;
            if (!variables.Add(Variable(node.GetData(), typeIndex, pushOffset))) return "";
            return AllocStack(size);
        }
        case NodeType::FunctionDefinition: {
            // TODO: Load arguments
            const MathLib::String name = node.GetData();
            Compiler function = Compiler(this);
            const MathLib::String ret = "global "_M + name + '\n' + name + ":\npush rbp\nmov rbp, rsp\n" + function.CompileCompound(node.At(1));
            return ret + function.CompileCompound(node.At(2)) + returnInstruction;
        }
        case NodeType::FunctionCall: {
            // TODO: Pass arguments
            return "call "_M + node.GetData() + "\nmov [rsp], rax\n" + AllocStack(wordSize);
        }
        default: return "";
    }
}
MathLib::String Compiler::Evaluate(const Node& node) {
    if (parent || !variables.Reset() || !types.Reset() || !types.Add(Type("uint64"_M, sizeof(uint64_t)))) return "";
    pushOffset = 0;
    return "bits 64\nsection .text\nReturn:\nmov rax, [rsp + 8]\nmov rsp, rbp\npop rbp\nret\nglobal _start\n_start:\ncall Main\nmov rdi, rax\nmov rax, 60\nsyscall\n"_M + CompileCompound(node);
}