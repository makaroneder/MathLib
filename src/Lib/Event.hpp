#ifndef MathLib_Event_H
#define MathLib_Event_H
#include "Math/Matrix.hpp"

namespace MathLib {
    struct Event : Printable {
        enum class Type : uint8_t {
            None,
            Quit,
            KeyPressed,
            MousePressed,
        };
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
        char key;

        Event(Type type = Type::None);
        Event(char key, bool pressed);
        Event(size_t mouseX, size_t mouseY, MouseButton button, bool pressed);
        Event(size_t mouseX, size_t mouseY);
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
        [[nodiscard]] bool operator==(const Event& other) const;
        [[nodiscard]] bool operator!=(const Event& other) const;
    };
}

#endif