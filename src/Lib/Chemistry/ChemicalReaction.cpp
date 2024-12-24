#include "ChemicalReaction.hpp"

namespace MathLib {
    ChemicalReaction::ChemicalReaction(void) {
        EmptyBenchmark
    }
    ChemicalReaction::ChemicalReaction(const Array<ChemicalMolecule>& left, const Array<ChemicalMolecule>& right) : left(left), right(right) {
        EmptyBenchmark
    }
    Expected<ChemicalReaction> ChemicalReaction::Create(const Array<ChemicalMolecule>& input) {
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
                    elements.Add(ChemicalElement(hydroxide.At(i), hydroxide.At(i).GetCount() * acid.At(0).GetCount()));
                for (size_t i = 1; i < acid.GetSize(); i++)
                    elements.Add(ChemicalElement(acid.At(i), acid.At(i).GetCount() * hydroxide.At(hydroxide.GetSize() - 1).GetCount()));
                ret.Add(ChemicalMolecule(elements, 1));
                Array<ChemicalElement> arr = Array<ChemicalElement>(2);
                arr.At(0) = Hydrogen(2);
                arr.At(1) = Oxygen(1);
                ret.Add(ChemicalMolecule(arr, 1));
            }
            else if ((input.At(0).IsMetal() && input.At(1).IsNonMetal()) || (input.At(1).IsMetal() && input.At(0).IsNonMetal())) {
                Array<ChemicalElement> arr = Array<ChemicalElement>(2);
                arr.At(0) = ChemicalElement((input.At(0).IsMetal() ? l0 : l1).At(0), 1);
                arr.At(1) = ChemicalElement((input.At(0).IsNonMetal() ? l0 : l1).At(0), 1);
                ret.Add(ChemicalMolecule(arr, 1));
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
                    elements.Add(rElement);
                }
            }
        }
        ReturnFromBenchmark(elements);
    }
    Expected<ChemicalReaction> ChemicalReaction::Balance(void) const {
        StartBenchmark
        const Array<ChemicalReactionElement> l = GetReactionElements(true);
        const Array<ChemicalReactionElement> r = GetReactionElements(false);
        const size_t size = left.GetSize() + right.GetSize() - 1;
        Matrix<num_t> a = Matrix<num_t>(size, size);
        Matrix<num_t> b = Matrix<num_t>(1, size);
        size_t sub = 0;
        for (size_t y = 0; y < l.GetSize(); y++) {
            if ((y - sub) == size) break;
            Matrix<ssize_t> v = l.At(y).coefficients;
            const size_t w = v.GetWidth();
            for (size_t x = 0; x < w; x++) a.At(x, y - sub) = v.At(x, 0);
            for (const ChemicalReactionElement& elment : r) {
                if (elment.GetSymbol() == l.At(y).GetSymbol()) {
                    v = elment.coefficients;
                    break;
                }
            }
            for (size_t x = 0; x < v.GetWidth() - 1; x++) a.At(x + w, y - sub) = -v.At(x, 0);
            b.At(0, y - sub) = v.At(v.GetWidth() - 1, 0);
            Matrix<num_t> tmp = Matrix<num_t>(size, 1);
            for (size_t j = 0; j < size; j++) tmp.At(j, 0) = a.At(j, y - sub);
            for (size_t i = 0; i < (y - sub); i++) {
                Matrix<num_t> a1 = Matrix<num_t>(size, 1);
                for (size_t j = 0; j < size; j++) a1.At(j, 0) = a.At(j, i);
                if (a1.IsMultipleOf(tmp)) {
                    sub++;
                    break;
                }
            }
        }
        Expected<Matrix<num_t>> tmp = a.GetInverse();
        if (!tmp.HasValue()) ReturnFromBenchmark(Expected<ChemicalReaction>());
        tmp = tmp.Get() * b;
        if (!tmp.HasValue()) ReturnFromBenchmark(Expected<ChemicalReaction>());
        const Matrix<num_t> x = tmp.Get();
        for (size_t i = 1; true; i++) {
            tmp = x * i;
            if (!tmp.HasValue()) ReturnFromBenchmark(Expected<ChemicalReaction>());
            const Matrix<num_t> tmpMat = tmp.Get();
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
    String ChemicalReaction::ToString(const String& padding) const {
        StartBenchmark
        String ret = padding;
        for (size_t i = 0; i < left.GetSize(); i++)
            ret += left.At(i).ToString() + ((i + 1) == left.GetSize() ? "" : " + ");
        ret += " => ";
        for (size_t i = 0; i < right.GetSize(); i++)
            ret += right.At(i).ToString() + ((i + 1) == right.GetSize() ? "" : " + ");
        ReturnFromBenchmark(ret);
    }
}