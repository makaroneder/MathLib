#include "Action.hpp"

Action::Action(void) : prefix(), type(Type::Ignore) {}
Action::Action(Type type, const MathLib::Sequence<char>& prefix) : prefix(MathLib::CollectionToString(prefix)), type(type) {}
MathLib::String Action::ToString(const MathLib::Sequence<char>& padding) const {
    MathLib::String ret = MathLib::CollectionToString(padding) + '[';
    switch (type) {
        case Type::Ignore: {
            ret += "Ignore";
            break;
        }
        case Type::Scrape: {
            ret += "Scrape";
            break;
        }
        case Type::Save: {
            ret += "Save";
            break;
        }
        case Type::IgnoreException: {
            ret += "IgnoreException";
            break;
        }
        default: break;
    }
    return ret + "] " + prefix;
}
bool Action::LessThanEqual(const MathLib::Orderable& other) const {
    return prefix.GetSize() <= ((const Action&)other).prefix.GetSize();
}