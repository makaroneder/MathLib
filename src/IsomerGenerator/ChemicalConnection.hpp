#ifndef ChemicalConnection_H
#define ChemicalConnection_H
#include <Expected.hpp>

struct ChemicalConnection : Printable {
    Array<ChemicalConnection> connections;

    ChemicalConnection(void);
    ChemicalConnection(const String& symbol, size_t valence);
    String GetSymbol(void) const;
    bool Connect(const ChemicalConnection& element);
    size_t GetFreeConnections(void) const;
    Array<size_t> ToNonHierarchicalData(void) const;
    bool IsLinear(void) const;
    size_t GetSubLists(void) const;
    ChemicalConnection* GetLast(void);
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual String ToString(const String& padding = "") const override;

    private:
    String symbol;
    size_t valence;
};

#endif