#include "ChemicalConnection.hpp"
#include <MathLib.hpp>
#include <iostream>

Array<ChemicalConnection> CreateConnections(const ChemicalConnection& connection, size_t firstFreeConnection) {
    Array<ChemicalConnection> ret;
    if (firstFreeConnection != connection.GetFreeConnections()) {
        ChemicalConnection tmp = connection;
        if (!tmp.Connect(ChemicalConnection(Carbon::symbol, 4)) || !ret.Add(tmp)) return Array<ChemicalConnection>();
    }
    Array<size_t> done;
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
            const Array<ChemicalConnection> tmp = CreateConnections(connection.connections.At(i), 1);
            for (const ChemicalConnection& x : tmp) {
                ChemicalConnection tmp = connection;
                tmp.connections.At(i) = x;
                if (!ret.Add(tmp)) return Array<ChemicalConnection>();
            }
        }
    }
    return ret;
}
void AddHydrogens(ChemicalConnection& connection, size_t firstFreeConnection) {
    const size_t iters = connection.GetFreeConnections() - firstFreeConnection;
    for (size_t i = 0; i < iters; i++) connection.Connect(ChemicalConnection(Hydrogen::symbol, 1));
    for (ChemicalConnection& conn : connection.connections) AddHydrogens(conn, 1);
}
Array<ChemicalConnection> CreateComponent(size_t carbons) {
    if (!carbons) return Array<ChemicalConnection>();
    Array<ChemicalConnection> ret = Array<ChemicalConnection>(1);
    ret.At(0) = ChemicalConnection(Carbon::symbol, 4);
    for (size_t i = 1; i < carbons; i++) {
        Array<ChemicalConnection> swap = Array<ChemicalConnection>();
        for (ChemicalConnection& root : ret) {
            const Array<ChemicalConnection> tmp = CreateConnections(root, 0);
            for (const ChemicalConnection& x : tmp) swap.Add(x);
        }
        ret = swap;
    }
    for (ChemicalConnection& root : ret) AddHydrogens(root, 0);
    Array<ChemicalConnection> reduced;
    for (const ChemicalConnection& root1 : ret) {
        const Array<size_t> data = BubbleSort<size_t>(root1.ToNonHierarchicalData());
        bool found = false;
        for (const ChemicalConnection& root2 : reduced) {
            if (data == BubbleSort<size_t>(root2.ToNonHierarchicalData())) {
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
        #ifdef Debug
        const Test test = TestSelf();
        const size_t tests = test.GetRecordCount();
        const size_t passed = test.GetPassed();
        std::cout << test << passed << "/" << tests << " tests passed" << std::endl;
        if (passed != tests) Panic("Some tests failed");
        #endif
        if (argc < 2) Panic(String("Usage: ") + argv[0] + " <carbons>");
        const Array<ChemicalConnection> connections = CreateComponent(StringToNumber(argv[1]));
        for (const ChemicalConnection& root : connections) std::cout << root.ToString() << '\n';
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}
