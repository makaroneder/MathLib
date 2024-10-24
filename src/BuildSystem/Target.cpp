#include "Target.hpp"

Target::Target(const String& name, const String& deps, const String& command, bool phony) : name(name), deps(deps), command(command), phony(phony) {}
String Target::ToString(const String& padding) const {
    return padding + name + ": " + deps + " => " + command;
}