#include "Event.hpp"

Event::Event(const Type& t) : type(t) {}
Event::Event(const char& chr, const bool& pressed) : Event(pressed ? Type::KeyPressed : Type::KeyReleased) {
    key = chr;
}