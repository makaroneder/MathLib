#ifndef Event_H
#define Event_H

/// @brief Renderer event
struct Event {
    /// @brief Type of event
    enum class Type {
        None,
        Quit,
        KeyPressed,
        KeyReleased,
    } type;
    /// @brief Data for different event types
    union Data {
        /// @brief Pressed or released key
        char key;
    } data;

    /// @brief Creates a new event
    /// @param t Type of event
    Event(Type t = Type::None);
    /// @brief Creates a new event
    /// @param chr Key pressed or released
    /// @param pressed Key status
    Event(char chr, bool pressed);
};

#endif