#ifndef Event_H
#define Event_H
#include "Interfaces/Printable.hpp"

/// @brief Renderer event
struct Event : Printable {
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
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual String ToString(const String& padding = "") const override;

    private:
    String KeyToString(void) const;
};

#endif