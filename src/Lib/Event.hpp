#ifndef Event_H
#define Event_H
#include "Math/Matrix.hpp"

namespace MathLib {
    /// @brief Renderer event
    struct Event : Printable {
        /// @brief Type of event
        enum class Type {
            None,
            Quit,
            KeyPressed,
            MousePressed,
        } type;
        enum class MouseButton {
            Left = 1,
            Middle,
            Right,
            Button4,
            Button5,
        };
        bool pressed;
        /// @brief Pressed or released key
        char key;
        /// @brief Mouse position
        Matrix<size_t> mousePosition;
        /// @brief Pressed or released mouse button
        MouseButton mouseButton;

        /// @brief Creates a new event
        /// @param type Type of event
        Event(Type type = Type::None);
        /// @brief Creates a new event
        /// @param key Key pressed or released
        /// @param pressed Key status
        Event(char key, bool pressed);
        /// @brief Creates a new event
        /// @param key Key pressed or released
        /// @param pressed Key status
        Event(const Matrix<size_t>& position, MouseButton button, bool pressed);
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        virtual String ToString(const String& padding = "") const override;
    };
}

#endif