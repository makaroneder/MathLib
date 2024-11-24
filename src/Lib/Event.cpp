#include "Event.hpp"

namespace MathLib {
    Event::Event(Type type) : type(type) {}
    Event::Event(char key, bool pressed) : type(Type::KeyPressed), pressed(pressed), key(key) {}
    Event::Event(const Matrix<size_t>& position, MouseButton button, bool pressed) : type(Type::MousePressed), pressed(pressed), mousePosition(position), mouseButton(button) {}
    String Event::ToString(const String& padding) const {
        switch (type) {
            case Event::Type::None: return padding + "None";
            case Event::Type::Quit: return padding + "Quit";
            case Event::Type::KeyPressed: return padding + (pressed ? "Pressed" : "Released") + " '" + (key == '\n' ? "\\n" : String(key)) + '\'';
            case Event::Type::MousePressed: {
                String ret = padding + (pressed ? "Pressed" : "Released") + ' ';
                switch (mouseButton) {
                    case MouseButton::Left: {
                        ret += "left";
                        break;
                    }
                    case MouseButton::Middle: {
                        ret += "middle";
                        break;
                    }
                    case MouseButton::Right: {
                        ret += "right";
                        break;
                    }
                    case MouseButton::Button4: {
                        ret += "4";
                        break;
                    }
                    case MouseButton::Button5: {
                        ret += "5";
                        break;
                    }
                    default: ret += "unknown";
                }
                return ret + " button at " + mousePosition.ToString();
            }
            default: return padding + "Unknown";
        }
    }
}