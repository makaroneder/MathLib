#include "Event.hpp"

namespace MathLib {
    Event::Event(Type type) : mousePosition(), type(type), mouseButton(MouseButton::None), pressed(), key() {
        EmptyBenchmark
    }
    Event::Event(char key, bool pressed) : mousePosition(), type(Type::KeyPressed), mouseButton(MouseButton::None), pressed(pressed), key(key) {
        EmptyBenchmark
    }
    Event::Event(const Matrix<size_t>& position, MouseButton button, bool pressed) : mousePosition(position), type(Type::MousePressed), mouseButton(button), pressed(pressed), key() {
        EmptyBenchmark
    }
    Event::Event(const Matrix<size_t>& position) : Event(position, MouseButton::None, false) {
        EmptyBenchmark
    }
    String Event::ToString(const Sequence<char>& padding) const {
        StartBenchmark
        switch (type) {
            case Event::Type::None: ReturnFromBenchmark(CollectionToString(padding) + "None");
            case Event::Type::Quit: ReturnFromBenchmark(CollectionToString(padding) + "Quit");
            case Event::Type::KeyPressed: ReturnFromBenchmark(CollectionToString(padding) + (pressed ? "Pressed" : "Released") + " '" + (key == '\n' ? "\\n" : String(key)) + '\'');
            case Event::Type::MousePressed: {
                String ret = CollectionToString(padding) + (pressed ? "Pressed" : "Released") + ' ';
                switch (mouseButton) {
                    case MouseButton::None: {
                        ret += "no";
                        break;
                    }
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
                ReturnFromBenchmark(ret + " button at " + mousePosition.ToString());
            }
            default: ReturnFromBenchmark(CollectionToString(padding) + "Unknown");
        }
    }
    bool Event::operator==(const Event& other) const {
        StartAndReturnFromBenchmark(type == other.type && mouseButton == other.mouseButton && pressed == other.pressed && key == other.key && mousePosition == other.mousePosition);
    }
    bool Event::operator!=(const Event& other) const {
        StartAndReturnFromBenchmark(!(*this == other));
    }
}