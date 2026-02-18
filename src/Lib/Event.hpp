#ifndef MathLib_Event_H
#define MathLib_Event_H
#include "Math/Matrix.hpp"

namespace MathLib {
    /// @brief Renderer event
    struct Event : Printable {
        /// @brief Type of event
        enum class Type : uint8_t {
            None,
            Quit,
            KeyPressed,
            MousePressed,
        };
        /// @brief Pressed or released mouse button
        enum class MouseButton : uint8_t {
            None = 0,
            Left,
            Middle,
            Right,
            Button4,
            Button5,
        };
        size_t mouseX;
        size_t mouseY;
        Type type;
        MouseButton mouseButton;
        bool pressed;
        /// @brief Pressed or released key
        char key;

        /// @brief Creates a new event
        /// @param type Type of event
        Event(Type type = Type::None);
        /// @brief Creates a new event
        /// @param key Key pressed or released
        /// @param pressed Key status
        Event(char key, bool pressed);
        /// @brief Creates a new event
        /// @param position Mouse position
        /// @param button Mouse button pressed or released
        /// @param pressed Mouse button status
        Event(size_t mouseX, size_t mouseY, MouseButton button, bool pressed);
        /// @brief Creates a new event
        /// @param position Mouse position
        Event(size_t mouseX, size_t mouseY);
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
        [[nodiscard]] bool operator==(const Event& other) const;
        [[nodiscard]] bool operator!=(const Event& other) const;
    };
}

#endif