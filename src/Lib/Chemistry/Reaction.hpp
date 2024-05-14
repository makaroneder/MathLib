#ifndef Chemistry_Reaction_H
#define Chemistry_Reaction_H
#include "Molecule.hpp"
#include "ReactionElement.hpp"

template <typename T>
struct ChemicalReaction : Printable  {
    ChemicalReaction<T>(std::vector<ChemicalMolecule<T>> l, std::vector<ChemicalMolecule<T>> r) : left(l), right(r) {}
    static constexpr ChemicalReaction<T> Create(std::vector<ChemicalMolecule<T>> l) {
        std::vector<ChemicalMolecule<T>> ret;
        if (l.size() == 2) {
            std::vector<ChemicalElement<T>> l0 = l.at(0).GetElements();
            std::vector<ChemicalElement<T>> l1 = l.at(1).GetElements();
            if ((l.at(0).IsHydroxide() && l.at(1).IsAcid()) || (l.at(1).IsHydroxide() && l.at(0).IsAcid())) {
                std::vector<ChemicalElement<T>>& hydroxide = l.at(0).IsHydroxide() ? l0 : l1;
                std::vector<ChemicalElement<T>>& acid = l.at(0).IsAcid() ? l0 : l1;
                std::vector<ChemicalElement<T>> elements;
                for (size_t i = 0; i < hydroxide.size() - 2; i++)
                    elements.push_back(ChemicalElement<T>(hydroxide.at(i), hydroxide.at(i).GetCount() * acid.at(0).GetCount()));
                for (size_t i = 1; i < acid.size(); i++)
                    elements.push_back(ChemicalElement<T>(acid.at(i), acid.at(i).GetCount() * hydroxide.at(hydroxide.size() - 1).GetCount()));
                ret.push_back(ChemicalMolecule<T>(elements, 1));
                ret.push_back(ChemicalMolecule<T>({ Hydrogen<T>(2), Oxygen<T>(1), }, 1));
            }
            else if ((l.at(0).IsMetal() && l.at(1).IsNonMetal()) || (l.at(1).IsMetal() && l.at(0).IsNonMetal()))
                ret.push_back(ChemicalMolecule<T>({ ChemicalElement<T>((l.at(0).IsMetal() ? l0 : l1).at(0), 1), ChemicalElement<T>((l.at(0).IsNonMetal() ? l0 : l1).at(0), 1), }, 1));
        }
        return ChemicalReaction<T>(l, ret);
    }
    constexpr std::vector<std::vector<ChemicalElement<T>>> GetLeftList(void) const {
        std::vector<std::vector<ChemicalElement<T>>> ret;
        for (const ChemicalMolecule<T>& mol : left) ret.push_back(mol.GetElements());
        return ret;
    }
    constexpr std::vector<std::vector<ChemicalElement<T>>> GetRightList(void) const {
        std::vector<std::vector<ChemicalElement<T>>> ret;
        for (const ChemicalMolecule<T>& mol : right) ret.push_back(mol.GetElements());
        return ret;
    }
    constexpr size_t GetLeftElementCount(std::string symbol) const {
        return GetElementCount(GetLeftList(), symbol);
    }
    constexpr size_t GetRightElementCount(std::string symbol) const {
        return GetElementCount(GetRightList(), symbol);
    }
    std::vector<ChemicalReactionElement> GetReactionElements(std::vector<ChemicalMolecule<T>> molecules) const {
        std::vector<ChemicalReactionElement> elements;
        for (size_t i = 0; i < molecules.size(); i++) {
            std::vector<ChemicalElement<T>> chemElements = molecules[i].GetElements();
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
                    ChemicalReactionElement rElement = ChemicalReactionElement(element.GetSymbol(), molecules.size());
                    rElement.coefficients.At(i, 0) = element.GetCount();
                    elements.push_back(rElement);
                }
            }
        }
        return elements;
    }
    constexpr ChemicalReaction<T> Balance(void) {
        const std::vector<ChemicalReactionElement> l = GetReactionElements(left);
        const std::vector<ChemicalReactionElement> r = GetReactionElements(right);
        const size_t size = left.size() + right.size() - 1;
        Matrix<T> a = Matrix<T>(size, size);
        Matrix<T> b = Matrix<T>(1, size);
        size_t sub = 0;
        for (size_t y = 0; y < l.size(); y++) {
            if ((y - sub) == size) break;
            Matrix<ssize_t> v = l.at(y).coefficients;
            const size_t w = v.GetWidth();
            for (size_t x = 0; x < w; x++) a.At(x, y - sub) = v.At(x, 0);
            for (const ChemicalReactionElement& element : r) {
                if (element.symbol == l.at(y).symbol) {
                    v = element.coefficients;
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
        const Matrix<T> x = a.GetInverse() * b;
        for (T i = 1; true; i++) {
            const Matrix<T> tmp = x * i;
            bool ok = true;
            for (size_t y = 0; y < tmp.GetHeight() && ok; y++)
                if (!FloatsEqual<T>(tmp.At(0, y), std::round(tmp.At(0, y)))) ok = false;
            if (ok) {
                ChemicalReaction<T> ret = *this;
                for (size_t j = 0; j < ret.left.size(); j++) ret.left.at(j).count = std::round(tmp.At(0, j));
                for (size_t j = 0; j < ret.right.size() - 1; j++) ret.right.at(j).count = std::round(tmp.At(0, j + ret.left.size()));
                ret.right.at(ret.right.size() - 1).count = std::round(i);
                return ret;
            }
        }
        return ChemicalReaction<T>({}, {});
    }
    virtual std::string ToString(std::string padding = "") const override {
        std::string ret = padding;
        for (size_t i = 0; i < left.size(); i++)
            ret += left.at(i).ToString() + ((i + 1) == left.size() ? "" : " + ");
        ret += " => ";
        for (size_t i = 0; i < right.size(); i++)
            ret += right.at(i).ToString() + ((i + 1) == right.size() ? "" : " + ");
        return ret;
    }

    private:
    std::vector<ChemicalMolecule<T>> left;
    std::vector<ChemicalMolecule<T>> right;

    static constexpr size_t GetElementCount(std::vector<std::vector<ChemicalElement<T>>> molecules, std::string symbol) {
        size_t ret = 0;
        for (const std::vector<ChemicalElement<T>>& molecule : molecules) {
            for (const ChemicalElement<T>& element : molecule) {
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