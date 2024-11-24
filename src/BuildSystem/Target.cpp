#include "Target.hpp"

Target::Target(const MathLib::String& name, const MathLib::String& deps, const MathLib::String& command, bool phony) : name(name), deps(deps), command(command), phony(phony) {}
MathLib::String Target::ToString(const MathLib::String& padding) const {
    return padding + name + ": " + deps + " => " + command;
}