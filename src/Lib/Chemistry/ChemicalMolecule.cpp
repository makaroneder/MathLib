#include "ChemicalMolecule.hpp"

namespace MathLib {
    ChemicalMolecule::ChemicalMolecule(void) : count(0), elements(Array<ChemicalElement>()) {
        EmptyBenchmark
    }
    ChemicalMolecule::ChemicalMolecule(const Sequence<ChemicalElement>& elements, size_t count) : count(count), elements(CollectionToArray<ChemicalElement>(elements)) {
        EmptyBenchmark
    }
    Array<ChemicalElement> ChemicalMolecule::GetElements(void) const {
        StartBenchmark
        Array<ChemicalElement> ret = elements;
        for (ChemicalElement& element : ret) element *= count;
        ReturnFromBenchmark(ret);
    }
    bool ChemicalMolecule::IsCompound(void) const {
        StartAndReturnFromBenchmark(elements.GetSize() > 1);
    }
    bool ChemicalMolecule::IsWater(void) const {
        StartAndReturnFromBenchmark(IsCompound() && elements.At(0).GetSymbol() == "H" && elements.At(1).GetSymbol() == "O" && elements.At(0).GetCount() == 2 && elements.At(1).GetCount() == 1);
    }
    bool ChemicalMolecule::IsMetal(void) const {
        StartAndReturnFromBenchmark(!IsCompound() && elements.At(0).IsMetal());
    }
    bool ChemicalMolecule::IsNonMetal(void) const {
        StartAndReturnFromBenchmark(!IsCompound() && !elements.At(0).IsMetal());
    }
    bool ChemicalMolecule::IsAcid(void) const {
        StartAndReturnFromBenchmark(IsCompound() && !IsWater() && elements.At(0).GetSymbol() == "H");
    }
    bool ChemicalMolecule::IsHydroxide(void) const {
        StartAndReturnFromBenchmark(IsCompound() && !IsWater() && elements.At(elements.GetSize() - 2).GetSymbol() == "O" && elements.At(elements.GetSize() - 1).GetSymbol() == "H" && elements.At(elements.GetSize() - 2).GetCount() == elements.At(elements.GetSize() - 1).GetCount());
    }
    bool ChemicalMolecule::IsOxide(void) const {
        StartAndReturnFromBenchmark(IsCompound() && !IsWater() && elements.At(elements.GetSize() - 1).GetSymbol() == "O");
    }
    String ChemicalMolecule::ToString(const Sequence<char>& padding) const {
        StartBenchmark
        String str = "";
        for (const ChemicalElement& element : elements) str += element.ToString();
        ReturnFromBenchmark(CollectionToString(padding) + (count == 1 ? "" : MathLib::ToString(count)) + str);
    }
}