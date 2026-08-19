#ifndef MathLib_Executable_ELF_Executable_H
#define MathLib_Executable_ELF_Executable_H
#include "ELFRelocationWithAddend.hpp"
#include "ELFSymbol.hpp"

namespace MathLib {
    struct ELFExecutable;
    struct ELFExecutable : Comparable<ELFExecutable> {
        ELFExecutable(void);
        ELFExecutable(const Array<uint8_t>& data, uint64_t dynamicOffset, uintptr_t base);
        Array<uint8_t> GetData(void) const;
        uintptr_t GetBase(void) const;
        const char* GetString(size_t offset) const;
        uint32_t GetSymbols(void) const;
        ELFSymbol GetSymbol(size_t i) const;
        uint64_t GetRelas(void) const;
        ELFRelocationWithAddend GetRela(size_t i) const;
        uint64_t GetPLTs(void) const;
        ELFRelocationWithAddend GetPLT(size_t i) const;
        size_t GetDependencies(void) const;
        const char* GetDependency(size_t i) const;
        bool Relocate(const Sequence<ELFExecutable>& deps);
        [[nodiscard]] virtual bool Equals(const ELFExecutable& other) const override;

        private:
        uint64_t ResolveSymbol(const Sequence<char>& name, const Sequence<ELFExecutable>& deps) const;
        bool ApplyRelocation(const ELFRelocationWithAddend& rela, const Sequence<ELFExecutable>& deps) const;

        Array<size_t> dependencies;
        Array<uint8_t> data;
        uintptr_t base;
        uint64_t stringTable;
        uint64_t symbolTable;
        uint64_t rela;
        uint64_t relaSize;
        uint64_t plt;
        uint64_t pltSize;
        uint64_t hashTable;
    };
}

#endif