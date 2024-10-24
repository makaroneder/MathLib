#include "ChemicalConnection.hpp"
#include <Chemistry/ChemicalElement.hpp>
#include <String.hpp>

ChemicalConnection::ChemicalConnection(void) {}
ChemicalConnection::ChemicalConnection(const String& symbol, size_t valence) : connections(Array<ChemicalConnection>()), symbol(symbol), valence(valence) {}
String ChemicalConnection::GetSymbol(void) const {
    return symbol;
}
bool ChemicalConnection::Connect(const ChemicalConnection& element) {
    return GetFreeConnections() ? connections.Add(element) : false;
}
size_t ChemicalConnection::GetFreeConnections(void) const {
    return valence - connections.GetSize();
}
// TODO: Preserve some hierarchy required for chemical identification
Array<size_t> ChemicalConnection::ToNonHierarchicalData(void) const {
    Array<size_t> others;
    size_t other = 0;
    for (const ChemicalConnection& conn : connections) {
        if (conn.connections.GetSize()) {
            const Array<size_t> tmp = conn.ToNonHierarchicalData();
            for (const size_t& x : tmp) if (!others.Add(x)) return Array<size_t>();
        }
        else other++;
    }
    if (!others.Add(other)) return Array<size_t>();
    return others;
}
bool ChemicalConnection::IsLinear(void) const {
    bool gotSelf = false;
    for (const ChemicalConnection& connection : connections) {
        if (connection.symbol == symbol) {
            if (gotSelf) return false;
            gotSelf = true;
        }
    }
    return true;
}
size_t ChemicalConnection::GetSubLists(void) const {
    size_t ret = 0;
    for (const ChemicalConnection& connection : connections)
        if (connection.symbol == symbol) ret++;
    return ret;
}
ChemicalConnection* ChemicalConnection::GetLast(void) {
    ChemicalConnection* ret = this;
    while (true) {
        if (!ret->IsLinear()) return nullptr;
        bool found = false;
        for (ChemicalConnection& connection : ret->connections) {
            if (connection.symbol == ret->symbol) {
                ret = &connection;
                found = true;
                break;
            }
        }
        if (!found) return ret;
    }
}
String ChemicalConnection::ToString(const String& padding) const {
    String ret = "";
    Array<ChemicalElement> children;
    for (const ChemicalConnection& connection : connections) {
        if (connection.connections.IsEmpty() && connection.symbol != symbol) {
            bool found = false;
            for (ChemicalElement& child : children) {
                if (child.GetSymbol() == connection.symbol) {
                    child = ChemicalElement(child, child.GetCount() + 1);
                    found = true;
                    break;
                }
            }
            if (!found) children.Add(ChemicalElement(connection.symbol, 1, false));
        }
        else if (IsLinear()) ret += String('-') + connection.ToString();
        else ret += connection.ToString(padding + '\t') + '\n';
    }
    String add;
    for (const ChemicalElement& child : children) add += child.ToString();
    return padding + symbol + add + (IsLinear() ? ret : (ret.IsEmpty() ? "" : String(": {\n") + ret + padding + '}'));
}