#include "ELFExecutable.hpp"
#include "ELFDynamic.hpp"

namespace MathLib {
    ELFExecutable::ELFExecutable(void) : dependencies(), data(), base(0), stringTable(UINT64_MAX), symbolTable(UINT64_MAX), rela(UINT64_MAX), relaSize(UINT64_MAX), plt(UINT64_MAX), pltSize(UINT64_MAX), hashTable(UINT64_MAX) {}
    ELFExecutable::ELFExecutable(const Array<uint8_t>& data_, uint64_t dynamicOffset, uintptr_t base) : dependencies(), data(data_), base(base), stringTable(UINT64_MAX), symbolTable(UINT64_MAX), rela(UINT64_MAX), relaSize(0), plt(UINT64_MAX), pltSize(0), hashTable(UINT64_MAX) {
        for (const ELFDynamic* dynamic = (const ELFDynamic*)(data.GetValue() + dynamicOffset); dynamic->type != ELFDynamic::Type::None; dynamic++) switch(dynamic->type) {
            case ELFDynamic::Type::StringTable: {
                stringTable = dynamic->value;
                break;
            }
            case ELFDynamic::Type::SymbolTable: {
                symbolTable = dynamic->value;
                break;
            }
            case ELFDynamic::Type::RelocationWithAddend: {
                rela = dynamic->value;
                break;
            }
            case ELFDynamic::Type::RelocationWithAddendSize: {
                relaSize = dynamic->value / sizeof(ELFRelocationWithAddend);
                break;
            }
            case ELFDynamic::Type::PLT: {
                plt = dynamic->value;
                break;
            }
            case ELFDynamic::Type::PLTSize: {
                pltSize = dynamic->value / sizeof(ELFRelocationWithAddend);
                break;
            }
            case ELFDynamic::Type::SymbolHashTable: {
                hashTable = dynamic->value;
                break;
            }
            case ELFDynamic::Type::RequiredLibrary: {
                if (!dependencies.Add(dynamic->value)) Panic("Failed to add dependency");
                break;
            }
            default: break;
        }
    }
    Array<uint8_t> ELFExecutable::GetData(void) const {
        return data;
    }
    uintptr_t ELFExecutable::GetBase(void) const {
        return base;
    }
    const char* ELFExecutable::GetString(size_t offset) const {
        return (const char*)data.GetValue() + stringTable + offset;
    }
    uint32_t ELFExecutable::GetSymbols(void) const {
        return ((const uint32_t*)(data.GetValue() + hashTable))[1];
    }
    ELFSymbol ELFExecutable::GetSymbol(size_t i) const {
        return ((const ELFSymbol*)(data.GetValue() + symbolTable))[i];
    }
    uint64_t ELFExecutable::GetRelas(void) const {
        return relaSize;
    }
    ELFRelocationWithAddend ELFExecutable::GetRela(size_t i) const {
        return ((const ELFRelocationWithAddend*)(data.GetValue() + rela))[i];
    }
    uint64_t ELFExecutable::GetPLTs(void) const {
        return pltSize;
    }
    ELFRelocationWithAddend ELFExecutable::GetPLT(size_t i) const {
        return ((const ELFRelocationWithAddend*)(data.GetValue() + plt))[i];
    }
    size_t ELFExecutable::GetDependencies(void) const {
        return dependencies.GetSize();
    }
    const char* ELFExecutable::GetDependency(size_t i) const {
        return i < dependencies.GetSize() ? GetString(dependencies.AtUnsafe(i)) : nullptr;
    }
    bool ELFExecutable::Relocate(const Sequence<ELFExecutable>& deps) {
        for (size_t i = 0; i < relaSize; i++)
            if (!ApplyRelocation(GetRela(i), deps)) return false;
        for (size_t i = 0; i < pltSize; i++)
            if (!ApplyRelocation(GetPLT(i), deps)) return false;
        return true;
    }
    uint64_t ELFExecutable::ResolveSymbol(const Sequence<char>& name, const Sequence<ELFExecutable>& deps) const {
        const size_t size = deps.GetSize();
        for (size_t i = 0; i < size; i++) {
            const ELFExecutable exec = deps.AtUnsafe(i);
            const uint32_t symbols = exec.GetSymbols();
            for (uint64_t j = 0; j < symbols; j++) {
                const ELFSymbol symbol = exec.GetSymbol(j);
                if (symbol.name && name == String(exec.GetString(symbol.name))) return exec.base + symbol.value;
            }
        }
        return UINT64_MAX;
    }
    bool ELFExecutable::ApplyRelocation(const ELFRelocationWithAddend& rela, const Sequence<ELFExecutable>& deps) const {
        // TODO: More relocations
        uint64_t* const address = (uint64_t*)(data.GetValue() + rela.address);
        switch (rela.type) {
            case ELFRelocation::Type::None: return true;
            case ELFRelocation::Type::CreateGOT:
            case ELFRelocation::Type::CreatePLT: {
                *address = ResolveSymbol(MathLib::String(GetString(GetSymbol(rela.symbol).name)), deps);
                return true;
            }
            case ELFRelocation::Type::Relative: {
                *address = base + rela.addend;
                return true;
            }
            default: return false;
        }
    }
    bool ELFExecutable::Equals(const ELFExecutable& other) const {
        return data == other.data && stringTable == other.stringTable && symbolTable == other.symbolTable && rela == other.rela && relaSize == other.relaSize && plt == other.plt && pltSize == other.pltSize && hashTable == other.hashTable;
    }
}