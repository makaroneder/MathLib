#include "Event.hpp"

Event::Event(Type t) : type(t) {}
Event::Event(char chr, bool pressed) : Event(pressed ? Type::KeyPressed : Type::KeyReleased) {
    data.key = chr;
}