#ifndef Logger_H
#define Logger_H
#include "Interfaces/Writeable.hpp"

#define LogString(str) (logger && logger->Puts(str))
#define LogChar(chr) (logger && logger->Write<char>(chr))

extern Writeable* logger;

#endif