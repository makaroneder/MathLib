#include <Chemistry/ChemicalElement.hpp>
#include <Physics/SIUnits.hpp>
#include <Graph/Tree.hpp>
#include <String.hpp>
#include <iostream>

MathLib::Pair<MathLib::String, size_t> symbols[] = {
    MathLib::Pair<MathLib::String, size_t>(MathLib::Hydrogen::symbol, 1),
    MathLib::Pair<MathLib::String, size_t>(MathLib::Carbon::symbol, 4),
};
size_t uniqueName = 0;
[[nodiscard]] MathLib::String MakeUniqueName(void) {
    return MathLib::ToString(uniqueName++, 10);
}
[[nodiscard]] MathLib::Array<MathLib::Tree<size_t>> CreateConnections(const MathLib::Tree<size_t>& connection, size_t firstFreeConnection) {
    MathLib::Array<MathLib::Tree<size_t>> ret;
    const size_t size = connection.children.GetSize();
    if (firstFreeConnection != (symbols[connection.data].second - size)) {
        MathLib::Tree<size_t> tmp = connection;
        if (!tmp.Add(MathLib::Tree<size_t>(MakeUniqueName(), 1)) || !ret.Add(tmp)) return MathLib::Array<MathLib::Tree<size_t>>();
    }
    MathLib::Array<size_t> done;
    for (size_t i = 0; i < size; i++) {
        bool found = false;
        for (const size_t& j : done) {
            if (connection.children.At(i) == connection.children.At(j)) {
                found = true;
                break;
            }
        }
        if (!found) {
            if (!done.Add(i)) return MathLib::Array<MathLib::Tree<size_t>>();
            const MathLib::Array<MathLib::Tree<size_t>> tmp = CreateConnections(connection.children.At(i), 1);
            for (const MathLib::Tree<size_t>& x : tmp) {
                MathLib::Tree<size_t> tmp = connection;
                tmp.children.At(i) = x;
                if (!ret.Add(tmp)) return MathLib::Array<MathLib::Tree<size_t>>();
            }
        }
    }
    return ret;
}
[[nodiscard]] bool AddHydrogens(MathLib::Tree<size_t>& connection, size_t firstFreeConnection) {
    const size_t iters = symbols[connection.data].second - connection.children.GetSize() - firstFreeConnection;
    for (size_t i = 0; i < iters; i++)
        if (!connection.Add(MathLib::Tree<size_t>(MakeUniqueName(), 0))) return false;
    for (MathLib::Tree<size_t>& conn : connection.children)
        if (!AddHydrogens(conn, 1)) return false;
    return true;
}
[[nodiscard]] MathLib::Array<MathLib::Tree<size_t>> CreateComponent(size_t carbons) {
    if (!carbons) return MathLib::Array<MathLib::Tree<size_t>>();
    MathLib::Array<MathLib::Tree<size_t>> ret = MathLib::MakeArray<MathLib::Tree<size_t>>(MathLib::Tree<size_t>(MakeUniqueName(), 1));
    for (size_t i = 1; i < carbons; i++) {
        MathLib::Array<MathLib::Tree<size_t>> swap = MathLib::Array<MathLib::Tree<size_t>>();
        for (MathLib::Tree<size_t>& root : ret) {
            const MathLib::Array<MathLib::Tree<size_t>> tmp = CreateConnections(root, 0);
            for (const MathLib::Tree<size_t>& x : tmp)
                if (!swap.Add(x)) return MathLib::Array<MathLib::Tree<size_t>>();
        }
        ret = swap;
    }
    MathLib::Array<MathLib::Tree<size_t>> reduced;
    for (const MathLib::Tree<size_t>& root1 : ret) {
        bool found = false;
        for (const MathLib::Tree<size_t>& root2 : reduced) {
            const MathLib::Expected<bool> tmp = root1 == root2;
            if (!tmp.HasValue()) return MathLib::Array<MathLib::Tree<size_t>>();
            if (tmp.Get()) {
                found = true;
                break;
            }
        }
        if (!found && !reduced.Add(root1)) return MathLib::Array<MathLib::Tree<size_t>>();
    }
    for (MathLib::Tree<size_t>& root : reduced)
        if (!AddHydrogens(root, 0)) return MathLib::Array<MathLib::Tree<size_t>>();
    return reduced;
}
bool IsLinear(const MathLib::Tree<size_t>& tree) {
    MathLib::String noChildSymbol = "";
    MathLib::String childSymbol = "";
    for (const MathLib::Tree<size_t>& child : tree.children) {
        if (!child.children.IsEmpty()) {
            if (childSymbol.IsEmpty()) childSymbol = symbols[child.data].first;
            else return false;
        }
        else {
            if (noChildSymbol.IsEmpty()) noChildSymbol = symbols[child.data].first;
            else if (noChildSymbol != symbols[child.data].first) return false;
        }
    }
    return true;
}
MathLib::String ToChemicalString(const MathLib::Tree<size_t>& tree, const MathLib::Sequence<char>& padding = ""_M) {
    const MathLib::String padd = MathLib::CollectionToString(padding);
    MathLib::String ret = padd + symbols[tree.data].first;
    if (tree.children.IsEmpty()) return ret;
    const bool isLinear = IsLinear(tree);
    MathLib::Pair<MathLib::String, size_t> tmp = MathLib::Pair<MathLib::String, size_t>("", 0);
    for (const MathLib::Tree<size_t>& child : tree.children) {
        if (!child.children.IsEmpty()) continue;
        if (tmp.first.IsEmpty()) tmp.first = symbols[child.data].first;
        else if (tmp.first != symbols[child.data].first) continue;
        tmp.second++;
    }
    if (!tmp.first.IsEmpty()) {
        if (tmp.second == tree.children.GetSize()) return ret + tmp.first + MathLib::ToString(tmp.second, 10);
        ret += tmp.first + (tmp.second == 1 ? "" : MathLib::ToString(tmp.second, 10));
    }
    if (!isLinear) ret += ": {\n";
    MathLib::Array<MathLib::Pair<MathLib::String, size_t>> children;
    for (const MathLib::Tree<size_t>& child : tree.children) {
        if (symbols[child.data].first == tmp.first) continue;
        else if (isLinear) {
            const MathLib::String tmp = ToChemicalString(child, padd);
            ret += '-'_M + MathLib::SubString(tmp, padd.GetSize(), tmp.GetSize() - padd.GetSize());
        }
        else {
            const MathLib::String tmp = ToChemicalString(child, padd + '\t');
            bool found = false;
            for (MathLib::Pair<MathLib::String, size_t>& x : children) {
                if (x.first == tmp) {
                    x.second++;
                    found = true;
                    break;
                }
            }
            if (!found && !children.Add(MathLib::Pair<MathLib::String, size_t>(tmp, 1))) return "";
        }
    }
    for (const MathLib::Pair<MathLib::String, size_t>& child : children) {
        if (child.second == 1) ret += child.first + '\n';
        else {
            size_t i = 0;
            MathLib::SkipWhiteSpace(child.first, i);
            ret += MathLib::SubString(child.first, 0, i) + MathLib::ToString(child.second, 10) + '(' + MathLib::SubString(child.first, i, child.first.GetSize() - i) + ")\n";
        }
    }
    if (isLinear) return ret;
    if (children.GetSize() == 1) {
        size_t i = ret.Find(':');
        const MathLib::String name = MathLib::SubString(ret, 0, i);
        i += 3;
        MathLib::SkipWhiteSpace(ret, i);
        return name + '-' + MathLib::SubString(ret, i, ret.GetSize() - i - 1);
    }
    return ret + padding + '}';
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <carbons>");
        const MathLib::Second<MathLib::num_t> start = MathLib::GetTime();
        const MathLib::Array<MathLib::Tree<size_t>> connections = CreateComponent(MathLib::StringToNumber(MathLib::String(argv[1])));
        for (const MathLib::Tree<size_t>& root : connections) std::cout << ToChemicalString(root) << '\n';
        std::cout << "Isomers: " << connections.GetSize() << std::endl;
        std::cout << "Time: " << MathLib::Second<MathLib::num_t>(MathLib::GetTime()) - start << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}