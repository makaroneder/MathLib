#include "MathArch.hpp"

namespace MathLib {
    MathArch::MathArch(const Array<uint8_t>& mem) : Emulator(mem), pc(0), sp(0), a(0) {}
    size_t MathArch::EncodeOneByteInstruction(size_t i, MathArchOpcode opcode) {
        if (memory.GetSize() <= i) return 0;
        memory.At(i) = (uint8_t)opcode;
        return 1;
    }
    size_t MathArch::EncodeConstInstruction(size_t i, MathArchOpcode opcode, uint64_t value) {
        if (memory.GetSize() <= i + sizeof(uint64_t)) return 0;
        memory.At(i) = (uint8_t)opcode;
        const uint8_t* tmp = (const uint8_t*)&value;
        for (uint8_t x = 0; x < sizeof(uint64_t); x++) memory.At(i + x + 1) = tmp[x];
        return sizeof(uint64_t) + 1;
    }
    bool MathArch::Run(void) {
        while (pc.value < GetSize())
            if (!Step()) return false;
        return true;
    }
    bool MathArch::Step(void) {
        const Expected<MathArchOpcode> opcode = Fetch<MathArchOpcode>();
        if (!opcode.HasValue()) return false;
        switch (opcode.Get()) {
            case MathArchOpcode::Nop: return true;
            case MathArchOpcode::SetPCToConst: {
                const Expected<uint64_t> tmp = Fetch<uint64_t>();
                if (!tmp.HasValue()) return false;
                pc = tmp.Get();
                return true;
            }
            case MathArchOpcode::SetSPToConst: {
                const Expected<uint64_t> tmp = Fetch<uint64_t>();
                if (!tmp.HasValue()) return false;
                sp = tmp.Get();
                return true;
            }
            case MathArchOpcode::PushConst: {
                const Expected<uint64_t> tmp = Fetch<uint64_t>();
                if (!tmp.HasValue()) return false;
                return Push<uint64_t>(tmp.Get());
            }
            case MathArchOpcode::PushA: return Push<uint64_t>(a.value);
            case MathArchOpcode::PopToA: {
                const Expected<uint64_t> tmp = Pop<uint64_t>();
                if (!tmp.HasValue()) return false;
                a = tmp.Get();
                return true;
            }
            default: return false;
        }
    }
    String MathArch::ToString(const String& padding) const {
        String ret = padding + "{\n";
        ret += padding + "\tPC = " + pc.ToString() + '\n';
        ret += padding + "\tSP = " + sp.ToString() + '\n';
        ret += padding + "\tA = " + a.ToString() + '\n';
        return ret + padding + '}';
    }
}