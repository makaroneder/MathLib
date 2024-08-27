#ifndef Chemistry_Reaction_H
#define Chemistry_Reaction_H
#include "Molecule.hpp"
#include "ReactionElement.hpp"

template <typename T>
struct ChemicalReaction : Printable  {
    ChemicalReaction<T>(void) {}
    ChemicalReaction<T>(const Array<ChemicalMolecule<T>>& l, const Array<ChemicalMolecule<T>>& r) : left(l), right(r) {}
    static constexpr Expected<ChemicalReaction<T>> Create(const Array<ChemicalMolecule<T>>& l) {
        Array<ChemicalMolecule<T>> ret;
        if (l.GetSize() == 2) {
            Array<ChemicalElement<T>> l0 = l.At(0).GetElements();
            Array<ChemicalElement<T>> l1 = l.At(1).GetElements();
            if ((l.At(0).IsHydroxide() && l.At(1).IsAcid()) || (l.At(1).IsHydroxide() && l.At(0).IsAcid())) {
                Array<ChemicalElement<T>>& hydroxide = l.At(0).IsHydroxide() ? l0 : l1;
                Array<ChemicalElement<T>>& acid = l.At(0).IsAcid() ? l0 : l1;
                Array<ChemicalElement<T>> elements;
                for (size_t i = 0; i < hydroxide.GetSize() - 2; i++)
                    elements.Add(ChemicalElement<T>(hydroxide.At(i), hydroxide.At(i).GetCount() * acid.At(0).GetCount()));
                for (size_t i = 1; i < acid.GetSize(); i++)
                    elements.Add(ChemicalElement<T>(acid.At(i), acid.At(i).GetCount() * hydroxide.At(hydroxide.GetSize() - 1).GetCount()));
                ret.Add(ChemicalMolecule<T>(elements, 1));
                Array<ChemicalElement<num_t>> arr = Array<ChemicalElement<num_t>>(2);
                arr.At(0) = Hydrogen<T>(2);
                arr.At(1) = Oxygen<T>(1);
                ret.Add(ChemicalMolecule<T>(arr, 1));
            }
            else if ((l.At(0).IsMetal() && l.At(1).IsNonMetal()) || (l.At(1).IsMetal() && l.At(0).IsNonMetal())) {
                Array<ChemicalElement<num_t>> arr = Array<ChemicalElement<num_t>>(2);
                arr.At(0) = ChemicalElement<T>((l.At(0).IsMetal() ? l0 : l1).At(0), 1);
                arr.At(1) = ChemicalElement<T>((l.At(0).IsNonMetal() ? l0 : l1).At(0), 1);
                ret.Add(ChemicalMolecule<T>(arr, 1));
            }
        }
        return ret.GetSize() ? Expected<ChemicalReaction<T>>(ChemicalReaction<T>(l, ret)) : Expected<ChemicalReaction<T>>();
    }
    constexpr Array<Array<ChemicalElement<T>>> GetLeftList(void) const {
        Array<Array<ChemicalElement<T>>> ret;
        for (const ChemicalElement<T>& element : left) ret.Add(element.GetElements());
        return ret;
    }
    constexpr Array<Array<ChemicalElement<T>>> GetRightList(void) const {
        Array<Array<ChemicalElement<T>>> ret;
        for (const ChemicalElement<T>& element : right) ret.Add(element.GetElements());
        return ret;
    }
    constexpr size_t GetLeftElementCount(const String& symbol) const {
        return GetElementCount(GetLeftList(), symbol);
    }
    constexpr size_t GetRightElementCount(const String& symbol) const {
        return GetElementCount(GetRightList(), symbol);
    }
    Array<ChemicalReactionElement> GetReactionElements(const Array<ChemicalMolecule<T>>& molecules) const {
        Array<ChemicalReactionElement> elements;
        for (size_t i = 0; i < molecules.GetSize(); i++) {
            Array<ChemicalElement<T>> chemElements = molecules.At(i).GetElements();
            for (const ChemicalElement<T>& element : chemElements) {
                bool found = false;
                for (ChemicalReactionElement& rElement : elements) {
                    if (rElement.symbol == element.GetSymbol()) {
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
        return elements;
    }
    constexpr Expected<ChemicalReaction<T>> Balance(void) const {
        const Array<ChemicalReactionElement> l = GetReactionElements(left);
        const Array<ChemicalReactionElement> r = GetReactionElements(right);
        const size_t size = left.GetSize() + right.GetSize() - 1;
        Matrix<T> a = Matrix<T>(size, size);
        Matrix<T> b = Matrix<T>(1, size);
        size_t sub = 0;
        for (size_t y = 0; y < l.GetSize(); y++) {
            if ((y - sub) == size) break;
            Matrix<ssize_t> v = l.At(y).coefficients;
            const size_t w = v.GetWidth();
            for (size_t x = 0; x < w; x++) a.At(x, y - sub) = v.At(x, 0);
            for (const ChemicalReactionElement& elment : r) {
                if (elment.symbol == l.At(y).symbol) {
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
        Expected<Matrix<T>> tmp = a.GetInverse();
        if (!tmp.HasValue()) return Expected<ChemicalReaction<T>>();
        tmp = tmp.Get() * b;
        if (!tmp.HasValue()) return Expected<ChemicalReaction<T>>();
        const Matrix<T> x = tmp.Get();
        for (T i = 1; true; i++) {
            tmp = x * i;
            if (!tmp.HasValue()) return Expected<ChemicalReaction<T>>();
            const Matrix<T> tmpMat = tmp.Get();
            bool ok = true;
            for (size_t y = 0; y < tmpMat.GetHeight() && ok; y++)
                if (!FloatsEqual<T>(tmpMat.At(0, y), Round(tmpMat.At(0, y)))) ok = false;
            if (ok) {
                ChemicalReaction<T> ret = *this;
                for (size_t j = 0; j < ret.left.GetSize(); j++) ret.left.At(j).count = Round(tmpMat.At(0, j));
                for (size_t j = 0; j < ret.right.GetSize() - 1; j++) ret.right.At(j).count = Round(tmpMat.At(0, j + ret.left.GetSize()));
                ret.right.At(ret.right.GetSize() - 1).count = Round(i);
                return Expected<ChemicalReaction<T>>(ret);
            }
        }
        return Expected<ChemicalReaction<T>>();
    }
    virtual String ToString(const String& padding = "") const override {
        String ret = padding;
        for (size_t i = 0; i < left.GetSize(); i++)
            ret += left.At(i).ToString() + ((i + 1) == left.GetSize() ? "" : " + ");
        ret += " => ";
        for (size_t i = 0; i < right.GetSize(); i++)
            ret += right.At(i).ToString() + ((i + 1) == right.GetSize() ? "" : " + ");
        return ret;
    }

    private:
    Array<ChemicalMolecule<T>> left;
    Array<ChemicalMolecule<T>> right;

    static constexpr size_t GetElementCount(const Array<Array<ChemicalElement<T>>>& molecules, const String& symbol) {
        size_t ret = 0;
        for (const Array<ChemicalElement<T>>& elements : molecules) {
            for (const ChemicalElement<T>& element : elements) {
                if (element.GetSymbol() == symbol) {
                    ret += element.GetCount();
                    break;
                }
            }
        }
        return ret;
    }
};

#endif