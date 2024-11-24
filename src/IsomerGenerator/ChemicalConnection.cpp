#include "ChemicalConnection.hpp"
#include <Chemistry/ChemicalElement.hpp>
#include <String.hpp>

ChemicalConnection::ChemicalConnection(void) {}
ChemicalConnection::ChemicalConnection(const MathLib::String& symbol, size_t valence) : connections(MathLib::Array<ChemicalConnection>()), symbol(symbol), valence(valence) {}
MathLib::String ChemicalConnection::GetSymbol(void) const {
    return symbol;
}
bool ChemicalConnection::Connect(const ChemicalConnection& element) {
    return GetFreeConnections() ? connections.Add(element) : false;
}
size_t ChemicalConnection::GetFreeConnections(void) const {
    return valence - connections.GetSize();
}
// TODO: Preserve some hierarchy required for chemical identification
MathLib::Array<size_t> ChemicalConnection::ToNonHierarchicalData(void) const {
    MathLib::Array<size_t> others;
    size_t other = 0;
    for (const ChemicalConnection& conn : connections) {
        if (conn.connections.GetSize()) {
            const MathLib::Array<size_t> tmp = conn.ToNonHierarchicalData();
            for (const size_t& x : tmp) if (!others.Add(x)) return MathLib::Array<size_t>();
        }
        else other++;
    }
    if (!others.Add(other)) return MathLib::Array<size_t>();
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
MathLib::String ChemicalConnection::ToString(const MathLib::String& padding) const {
    MathLib::String ret = "";
    MathLib::Array<MathLib::ChemicalElement> children;
    for (const ChemicalConnection& connection : connections) {
        if (connection.connections.IsEmpty() && connection.symbol != symbol) {
            bool found = false;
            for (MathLib::ChemicalElement& child : children) {
                if (child.GetSymbol() == connection.symbol) {
                    child = MathLib::ChemicalElement(child, child.GetCount() + 1);
                    found = true;
                    break;
                }
            }
            if (!found) children.Add(MathLib::ChemicalElement(connection.symbol, 1, false));
        }
        else if (IsLinear()) ret += MathLib::String('-') + connection.ToString();
        else ret += connection.ToString(padding + '\t') + '\n';
    }
    MathLib::String add;
    for (const MathLib::ChemicalElement& child : children) add += child.ToString();
    return padding + symbol + add + (IsLinear() ? ret : (ret.IsEmpty() ? "" : MathLib::String(": {\n") + ret + padding + '}'));
}