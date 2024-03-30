#ifndef Chemistry_Reaction_H
#define Chemistry_Reaction_H
#include "Molecule.hpp"
#include <iostream>

template <typename T>
struct ChemicalReaction : Printable  {
    ChemicalReaction<T>(std::vector<ChemicalMolecule<num_t>> l, std::vector<ChemicalMolecule<num_t>> r) {
        left = l;
        right = r;
    }
    constexpr std::vector<std::vector<ChemicalElement<num_t>>> GetLeftList(void) const {
        std::vector<std::vector<ChemicalElement<num_t>>> ret;
        for (const ChemicalMolecule<num_t>& mol : left) ret.push_back(mol.GetElements());
        return ret;
    }
    constexpr std::vector<std::vector<ChemicalElement<num_t>>> GetRightList(void) const {
        std::vector<std::vector<ChemicalElement<num_t>>> ret;
        for (const ChemicalMolecule<num_t>& mol : right) ret.push_back(mol.GetElements());
        return ret;
    }
    constexpr size_t GetLeftElementCount(std::string symbol) const {
        return GetElementCount(GetLeftList(), symbol);
    }
    constexpr size_t GetRightElementCount(std::string symbol) const {
        return GetElementCount(GetRightList(), symbol);
    }
    constexpr void SetCount(std::vector<size_t> count) {
        for (size_t i = 0; i < count.size(); i++) {
            if (i < left.size()) left.at(i).count = count.at(i);
            else right.at(i - left.size()).count = count.at(i);
        }
    }
    constexpr bool BalanceReaction(size_t maxCoefficient) {
        const size_t size = left.size() + right.size();
        std::vector<size_t> count = std::vector<size_t>(size, 1);
        while (true) {
            ChemicalReaction<T> tmp = *this;
            tmp.SetCount(count);
            bool ok = true;
            for (auto& symbol : chemicalSymbols) {
                if (tmp.GetLeftElementCount(symbol) != tmp.GetRightElementCount(symbol)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                SetCount(count);
                return true;
            }
            ssize_t i;
            for (i = size - 1; i >= 0 && ++count[i] > maxCoefficient; i--) count[i] = 1;
            if (i < 0) return false;
        }
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
    std::vector<ChemicalMolecule<num_t>> left;
    std::vector<ChemicalMolecule<num_t>> right;

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