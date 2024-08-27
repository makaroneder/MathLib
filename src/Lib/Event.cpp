#include "Event.hpp"

Event::Event(const Type& t) : type(t) {}
Event::Event(const char& chr, const bool& pressed) : Event(pressed ? Type::KeyPressed : Type::KeyReleased) {
    key = chr;
}
String Event::ToString(const String& padding) const {
    switch (type) {
        case Event::Type::None: return padding + "None";
        case Event::Type::Quit: return padding + "Quit";
        case Event::Type::KeyPressed: return padding + "Pressed '" + key + '\'';
        case Event::Type::KeyReleased: return padding + "Released '" + key + '\'';
        default: return padding + "Unknown";
    }
}