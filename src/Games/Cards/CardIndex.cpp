#include "CardIndex.hpp"

CardIndex::CardIndex(Card::Type type, Card::Color color) : type(type), color(color), selected(false) {}
bool CardIndex::operator<(const CardIndex& other) const {
    return type < other.type;
}
MathLib::String CardIndex::ToString(const MathLib::Sequence<char>& padding) const {
    MathLib::String ret = MathLib::CollectionToString(padding);
    switch (type) {
        case Card::Type::C2: {
            ret += "2";
            break;
        }
        case Card::Type::C3: {
            ret += "3";
            break;
        }
        case Card::Type::C4: {
            ret += "4";
            break;
        }
        case Card::Type::C5: {
            ret += "5";
            break;
        }
        case Card::Type::C6: {
            ret += "6";
            break;
        }
        case Card::Type::C7: {
            ret += "7";
            break;
        }
        case Card::Type::C8: {
            ret += "8";
            break;
        }
        case Card::Type::C9: {
            ret += "9";
            break;
        }
        case Card::Type::C10: {
            ret += "10";
            break;
        }
        case Card::Type::Jack: {
            ret += "Jack";
            break;
        }
        case Card::Type::Queen: {
            ret += "Queen";
            break;
        }
        case Card::Type::King: {
            ret += "King";
            break;
        }
        case Card::Type::Ace: {
            ret += "Ace";
            break;
        }
        default: ret += "Unknown";
    }
    ret += " of ";
    switch (color) {
        case Card::Color::Clubs: {
            ret += "clubs";
            break;
        }
        case Card::Color::Diamonds: {
            ret += "diamonds";
            break;
        }
        case Card::Color::Hearts: {
            ret += "hearts";
            break;
        }
        case Card::Color::Spades: {
            ret += "spades";
            break;
        }
        default: ret += "unknown";
    }
    return ret + (selected ? " (selected)" : "");
}