#ifndef Logger_H
#define Logger_H
#include "Interfaces/Writeable.hpp"

#define LogString(str) (MathLib::logger && MathLib::logger->Puts(str))
#define LogChar(chr) (MathLib::logger && MathLib::logger->Write<char>(chr))

namespace MathLib {
    extern Writeable* logger;
}

#endif