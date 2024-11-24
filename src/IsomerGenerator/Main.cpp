#include "ChemicalConnection.hpp"
#include <MathLib.hpp>
#include <iostream>

MathLib::Array<ChemicalConnection> CreateConnections(const ChemicalConnection& connection, size_t firstFreeConnection) {
    MathLib::Array<ChemicalConnection> ret;
    if (firstFreeConnection != connection.GetFreeConnections()) {
        ChemicalConnection tmp = connection;
        if (!tmp.Connect(ChemicalConnection(MathLib::Carbon::symbol, 4)) || !ret.Add(tmp)) return MathLib::Array<ChemicalConnection>();
    }
    MathLib::Array<size_t> done;
    for (size_t i = 0; i < connection.connections.GetSize(); i++) {
        bool found = false;
        for (const size_t& j : done) {
            if (connection.connections.At(i).ToString() == connection.connections.At(j).ToString()) {
                found = true;
                break;
            }
        }
        if (!found) {
            done.Add(i);
            const MathLib::Array<ChemicalConnection> tmp = CreateConnections(connection.connections.At(i), 1);
            for (const ChemicalConnection& x : tmp) {
                ChemicalConnection tmp = connection;
                tmp.connections.At(i) = x;
                if (!ret.Add(tmp)) return MathLib::Array<ChemicalConnection>();
            }
        }
    }
    return ret;
}
void AddHydrogens(ChemicalConnection& connection, size_t firstFreeConnection) {
    const size_t iters = connection.GetFreeConnections() - firstFreeConnection;
    for (size_t i = 0; i < iters; i++) connection.Connect(ChemicalConnection(MathLib::Hydrogen::symbol, 1));
    for (ChemicalConnection& conn : connection.connections) AddHydrogens(conn, 1);
}
MathLib::Array<ChemicalConnection> CreateComponent(size_t carbons) {
    if (!carbons) return MathLib::Array<ChemicalConnection>();
    MathLib::Array<ChemicalConnection> ret = MathLib::MakeArrayFromSingle<ChemicalConnection>(ChemicalConnection(MathLib::Carbon::symbol, 4));
    for (size_t i = 1; i < carbons; i++) {
        MathLib::Array<ChemicalConnection> swap = MathLib::Array<ChemicalConnection>();
        for (ChemicalConnection& root : ret) {
            const MathLib::Array<ChemicalConnection> tmp = CreateConnections(root, 0);
            for (const ChemicalConnection& x : tmp) swap.Add(x);
        }
        ret = swap;
    }
    for (ChemicalConnection& root : ret) AddHydrogens(root, 0);
    MathLib::Array<ChemicalConnection> reduced;
    for (const ChemicalConnection& root1 : ret) {
        const MathLib::Array<size_t> data = MathLib::BubbleSort<size_t>(root1.ToNonHierarchicalData());
        bool found = false;
        for (const ChemicalConnection& root2 : reduced) {
            if (data == MathLib::BubbleSort<size_t>(root2.ToNonHierarchicalData())) {
                found = true;
                break;
            }
        }
        if (!found) reduced.Add(root1);
    }
    return reduced;
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 2) MathLib::Panic(MathLib::String("Usage: ") + argv[0] + " <carbons>");
        const MathLib::Array<ChemicalConnection> connections = CreateComponent(MathLib::StringToNumber(argv[1]));
        for (const ChemicalConnection& root : connections) std::cout << root << '\n';
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}