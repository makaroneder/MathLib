#include "ChemicalReaction.hpp"
#include "../Math/SystemOfLinearEquations.hpp"

namespace MathLib {
    ChemicalReaction::ChemicalReaction(void) {
        EmptyBenchmark
    }
    ChemicalReaction::ChemicalReaction(const Sequence<ChemicalMolecule>& left, const Sequence<ChemicalMolecule>& right) : left(CollectionToArray<ChemicalMolecule>(left)), right(CollectionToArray<ChemicalMolecule>(right)) {
        EmptyBenchmark
    }
    Expected<ChemicalReaction> ChemicalReaction::Create(const Sequence<ChemicalMolecule>& input) {
        StartBenchmark
        Array<ChemicalMolecule> ret;
        if (input.GetSize() == 2) {
            Array<ChemicalElement> l0 = input.At(0).GetElements();
            Array<ChemicalElement> l1 = input.At(1).GetElements();
            if ((input.At(0).IsHydroxide() && input.At(1).IsAcid()) || (input.At(1).IsHydroxide() && input.At(0).IsAcid())) {
                Array<ChemicalElement>& hydroxide = input.At(0).IsHydroxide() ? l0 : l1;
                Array<ChemicalElement>& acid = input.At(0).IsAcid() ? l0 : l1;
                Array<ChemicalElement> elements;
                for (size_t i = 0; i < hydroxide.GetSize() - 2; i++)
                    if (!elements.Add(ChemicalElement(hydroxide.At(i), hydroxide.At(i).GetCount() * acid.At(0).GetCount()))) ReturnFromBenchmark(Expected<ChemicalReaction>());
                for (size_t i = 1; i < acid.GetSize(); i++)
                    if (!elements.Add(ChemicalElement(acid.At(i), acid.At(i).GetCount() * hydroxide.At(hydroxide.GetSize() - 1).GetCount()))) ReturnFromBenchmark(Expected<ChemicalReaction>());
                if (!ret.Add(ChemicalMolecule(elements, 1))) ReturnFromBenchmark(Expected<ChemicalReaction>());
                Array<ChemicalElement> arr = Array<ChemicalElement>(2);
                arr.At(0) = Hydrogen(2);
                arr.At(1) = Oxygen(1);
                if (!ret.Add(ChemicalMolecule(arr, 1))) ReturnFromBenchmark(Expected<ChemicalReaction>());
            }
            else if ((input.At(0).IsMetal() && input.At(1).IsNonMetal()) || (input.At(1).IsMetal() && input.At(0).IsNonMetal())) {
                Array<ChemicalElement> arr = Array<ChemicalElement>(2);
                arr.At(0) = ChemicalElement((input.At(0).IsMetal() ? l0 : l1).At(0), 1);
                arr.At(1) = ChemicalElement((input.At(0).IsNonMetal() ? l0 : l1).At(0), 1);
                if (!ret.Add(ChemicalMolecule(arr, 1))) ReturnFromBenchmark(Expected<ChemicalReaction>());
            }
        }
        ReturnFromBenchmark(ret.GetSize() ? Expected<ChemicalReaction>(ChemicalReaction(input, ret)) : Expected<ChemicalReaction>());
    }
    Array<ChemicalReactionElement> ChemicalReaction::GetReactionElements(bool left_) const {
        StartBenchmark
        Array<ChemicalReactionElement> elements;
        Array<ChemicalMolecule> molecules = left_ ? left : right;
        for (size_t i = 0; i < molecules.GetSize(); i++) {
            Array<ChemicalElement> chemElements = molecules.At(i).GetElements();
            for (const ChemicalElement& element : chemElements) {
                bool found = false;
                for (ChemicalReactionElement& rElement : elements) {
                    if (rElement.GetSymbol() == element.GetSymbol()) {
                        rElement.coefficients.At(i, 0) = element.GetCount();
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    ChemicalReactionElement rElement = ChemicalReactionElement(element.GetSymbol(), molecules.GetSize());
                    rElement.coefficients.At(i, 0) = element.GetCount();
                    if (!elements.Add(rElement)) ReturnFromBenchmark(Array<ChemicalReactionElement>());
                }
            }
        }
        ReturnFromBenchmark(elements);
    }
    Expected<ChemicalReaction> ChemicalReaction::Balance(void) const {
        StartBenchmark
        const Array<ChemicalReactionElement> l = GetReactionElements(true);
        const Array<ChemicalReactionElement> r = GetReactionElements(false);
        SystemOfLinearEquations<num_t> equations = SystemOfLinearEquations<num_t>(left.GetSize() + right.GetSize() - 1);
        for (size_t y = 0; y < l.GetSize(); y++) {
            for (const ChemicalReactionElement& elment : r) {
                if (elment.GetSymbol() == l.At(y).GetSymbol()) {
                    const size_t width1 = l.At(y).coefficients.GetWidth();
                    const size_t width2 = elment.coefficients.GetWidth() - 1;
                    matrix_t tmp = matrix_t(width1 + width2 + 1, 1);
                    for (size_t i = 0; i < width1; i++) tmp.At(i, 0) = l.At(y).coefficients.At(i, 0);
                    for (size_t i = 0; i < width2; i++) tmp.At(i + width1, 0) = -elment.coefficients.At(i, 0);
                    tmp.At(width1 + width2, 0) = elment.coefficients.At(width2, 0);
                    if (!equations.AddEquation(AugmentedMatrix<num_t>(tmp, tmp.GetWidth() - 1))) ReturnFromBenchmark(Expected<ChemicalReaction>());
                    break;
                }
            }
        }
        Expected<matrix_t> tmp = equations.GetMatrixSolution();
        if (!tmp.HasValue()) ReturnFromBenchmark(Expected<ChemicalReaction>());
        const matrix_t x = tmp.Get();
        for (size_t i = 1; true; i++) {
            tmp = x * i;
            if (!tmp.HasValue()) ReturnFromBenchmark(Expected<ChemicalReaction>());
            const matrix_t tmpMat = tmp.Get();
            bool ok = true;
            for (size_t y = 0; y < tmpMat.GetHeight() && ok; y++)
                if (!FloatsEqual<num_t>(tmpMat.At(0, y), Round(tmpMat.At(0, y)))) ok = false;
            if (ok) {
                ChemicalReaction ret = *this;
                for (size_t j = 0; j < ret.left.GetSize(); j++) ret.left.At(j).count = Round(tmpMat.At(0, j));
                for (size_t j = 0; j < ret.right.GetSize() - 1; j++) ret.right.At(j).count = Round(tmpMat.At(0, j + ret.left.GetSize()));
                ret.right.At(ret.right.GetSize() - 1).count = Round(i);
                ReturnFromBenchmark(Expected<ChemicalReaction>(ret));
            }
        }
        ReturnFromBenchmark(Expected<ChemicalReaction>());
    }
    String ChemicalReaction::ToString(const Sequence<char>& padding) const {
        StartBenchmark
        String ret = CollectionToString(padding);
        for (size_t i = 0; i < left.GetSize(); i++)
            ret += left.At(i).ToString() + ((i + 1) == left.GetSize() ? "" : " + ");
        ret += " => ";
        for (size_t i = 0; i < right.GetSize(); i++)
            ret += right.At(i).ToString() + ((i + 1) == right.GetSize() ? "" : " + ");
        ReturnFromBenchmark(ret);
    }
}