#ifndef ChemicalConnection_H
#define ChemicalConnection_H
#include <Expected.hpp>

struct ChemicalConnection : MathLib::Printable {
    MathLib::Array<ChemicalConnection> connections;

    ChemicalConnection(void);
    ChemicalConnection(const MathLib::String& symbol, size_t valence);
    MathLib::String GetSymbol(void) const;
    bool Connect(const ChemicalConnection& element);
    size_t GetFreeConnections(void) const;
    MathLib::Array<size_t> ToNonHierarchicalData(void) const;
    bool IsLinear(void) const;
    size_t GetSubLists(void) const;
    ChemicalConnection* GetLast(void);
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual MathLib::String ToString(const MathLib::String& padding = "") const override;

    private:
    MathLib::String symbol;
    size_t valence;
};

#endif