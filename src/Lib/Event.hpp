#ifndef Event_H
#define Event_H

struct Event {
    enum class Type {
        None,
        Quit,
        KeyPressed,
        KeyReleased,
    } type;
    union Data {
        char key;
    } data;

    Event(Type t = Type::None);
    Event(char chr, bool pressed);
};

#endif