#include "Event.hpp"

Event::Event(const Type& t) : type(t) {}
Event::Event(const char& chr, const bool& pressed) : Event(pressed ? Type::KeyPressed : Type::KeyReleased) {
    key = chr;
}
String Event::KeyToString(void) const {
    if (key == '\n') return "\\n";
    else return key;
}
String Event::ToString(const String& padding) const {
    switch (type) {
        case Event::Type::None: return padding + "None";
        case Event::Type::Quit: return padding + "Quit";
        case Event::Type::KeyPressed: return padding + "Pressed '" + KeyToString() + '\'';
        case Event::Type::KeyReleased: return padding + "Released '" + KeyToString() + '\'';
        default: return padding + "Unknown";
    }
}