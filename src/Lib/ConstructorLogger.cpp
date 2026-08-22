#include "ConstructorLogger.hpp"
#include "Logger.hpp"

namespace MathLib {
    size_t ConstructorLogger::currentID = 0;
    ConstructorLogger::ConstructorLogger(void) : id(currentID++) {
        LogChar('[');
        LogString(MathLib::ToString(id, 10));
        LogString("] constructed\n");
    }
    ConstructorLogger::ConstructorLogger(const ConstructorLogger& other) : id(currentID++) {
        LogChar('[');
        LogString(MathLib::ToString(id, 10));
        LogString("] copy constructed from [");
        LogString(MathLib::ToString(other.id, 10));
        LogString("]\n");
    }
    ConstructorLogger::ConstructorLogger(ConstructorLogger&& other) : id(currentID++) {
        LogChar('[');
        LogString(MathLib::ToString(id, 10));
        LogString("] move constructed from [");
        LogString(MathLib::ToString(other.id, 10));
        LogString("]\n");
    }
    ConstructorLogger::~ConstructorLogger(void) {
        LogChar('[');
        LogString(MathLib::ToString(id, 10));
        LogString("] deconstructed\n");
    }
    ConstructorLogger& ConstructorLogger::operator=(const ConstructorLogger& other) {
        LogChar('[');
        LogString(MathLib::ToString(id, 10));
        LogString("] copy assigned from [");
        LogString(MathLib::ToString(other.id, 10));
        LogString("]\n");
        return *this;
    }
    ConstructorLogger& ConstructorLogger::operator=(ConstructorLogger&& other) {
        LogChar('[');
        LogString(MathLib::ToString(id, 10));
        LogString("] move assigned from [");
        LogString(MathLib::ToString(other.id, 10));
        LogString("]\n");
        return *this;
    }
    bool ConstructorLogger::LessThanEqual(const ConstructorLogger& other) const {
        return id <= other.id;
    }
    String ConstructorLogger::ToString(const Sequence<char>&) const {
        return MathLib::ToString(id, 10);
    }
}