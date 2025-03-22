#include "Target.hpp"

Target::Target(const MathLib::Sequence<char>& name, const MathLib::Sequence<char>& deps, const MathLib::Sequence<char>& command, bool phony) : name(MathLib::CollectionToString(name)), deps(MathLib::CollectionToString(deps)), command(MathLib::CollectionToString(command)), phony(phony) {}
MathLib::String Target::ToString(const MathLib::Sequence<char>& padding) const {
    return MathLib::CollectionToString(padding) + name + ": " + deps + " => " + command;
}