#ifndef Event_H
#define Event_H
#include "Allocatable.hpp"

/// @brief Renderer event
struct Event : Allocatable {
    /// @brief Type of event
    enum class Type {
        None,
        Quit,
        KeyPressed,
        KeyReleased,
    } type;
    /// @brief Pressed or released key
    char key;

    /// @brief Creates a new event
    /// @param t Type of event
    Event(const Type& t = Type::None);
    /// @brief Creates a new event
    /// @param chr Key pressed or released
    /// @param pressed Key status
    Event(const char& chr, const bool& pressed);
};

#endif