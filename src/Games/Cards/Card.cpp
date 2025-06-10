#include "Card.hpp"

Card::Card(Type type, Color color) : type(type), color(color), selected(false) {}
bool Card::LessThanEqual(const MathLib::Orderable& other) const {
    return type <= ((const Card&)other).type;
}
MathLib::String Card::ToString(const MathLib::Sequence<char>& padding) const {
    MathLib::String ret = MathLib::CollectionToString(padding);
    switch (type) {
        case Type::C2: {
            ret += "2";
            break;
        }
        case Type::C3: {
            ret += "3";
            break;
        }
        case Type::C4: {
            ret += "4";
            break;
        }
        case Type::C5: {
            ret += "5";
            break;
        }
        case Type::C6: {
            ret += "6";
            break;
        }
        case Type::C7: {
            ret += "7";
            break;
        }
        case Type::C8: {
            ret += "8";
            break;
        }
        case Type::C9: {
            ret += "9";
            break;
        }
        case Type::C10: {
            ret += "10";
            break;
        }
        case Type::Jack: {
            ret += "Jack";
            break;
        }
        case Type::Queen: {
            ret += "Queen";
            break;
        }
        case Type::King: {
            ret += "King";
            break;
        }
        case Type::Ace: {
            ret += "Ace";
            break;
        }
        default: ret += "Unknown";
    }
    ret += " of ";
    switch (color) {
        case Color::Clubs: {
            ret += "clubs";
            break;
        }
        case Color::Diamonds: {
            ret += "diamonds";
            break;
        }
        case Color::Hearts: {
            ret += "hearts";
            break;
        }
        case Color::Spades: {
            ret += "spades";
            break;
        }
        default: ret += "unknown";
    }
    return ret + (selected ? " (selected)" : "");
}