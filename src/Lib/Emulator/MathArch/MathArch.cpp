#include "MathArch.hpp"

namespace MathLib {
    MathArch::MathArch(const Array<uint8_t>& mem) : Emulator(mem), pc(0), sp(0), a(0) {
        EmptyBenchmark
    }
    size_t MathArch::EncodeOneByteInstruction(size_t i, MathArchOpcode opcode) {
        StartBenchmark
        if (memory.GetSize() <= i) ReturnFromBenchmark(0);
        memory.At(i) = (uint8_t)opcode;
        ReturnFromBenchmark(1);
    }
    size_t MathArch::EncodeConstInstruction(size_t i, MathArchOpcode opcode, uint64_t value) {
        StartBenchmark
        if (memory.GetSize() <= i + sizeof(uint64_t)) ReturnFromBenchmark(0);
        memory.At(i) = (uint8_t)opcode;
        const uint8_t* tmp = (const uint8_t*)&value;
        for (uint8_t x = 0; x < sizeof(uint64_t); x++) memory.At(i + x + 1) = tmp[x];
        ReturnFromBenchmark(sizeof(uint64_t) + 1);
    }
    bool MathArch::Run(void) {
        StartBenchmark
        while (pc.value < GetSize())
            if (!Step()) ReturnFromBenchmark(false);
        ReturnFromBenchmark(true);
    }
    bool MathArch::Step(void) {
        StartBenchmark
        const Expected<MathArchOpcode> opcode = Fetch<MathArchOpcode>();
        if (!opcode.HasValue()) ReturnFromBenchmark(false);
        switch (opcode.Get()) {
            case MathArchOpcode::Nop: ReturnFromBenchmark(true);
            case MathArchOpcode::SetPCToConst: {
                const Expected<uint64_t> tmp = Fetch<uint64_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false);
                pc = tmp.Get();
                ReturnFromBenchmark(true);
            }
            case MathArchOpcode::SetSPToConst: {
                const Expected<uint64_t> tmp = Fetch<uint64_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false);
                sp = tmp.Get();
                ReturnFromBenchmark(true);
            }
            case MathArchOpcode::PushConst: {
                const Expected<uint64_t> tmp = Fetch<uint64_t>();
                ReturnFromBenchmark(tmp.HasValue() ? Push<uint64_t>(tmp.Get()) : false);
            }
            case MathArchOpcode::PushA: ReturnFromBenchmark(Push<uint64_t>(a.value));
            case MathArchOpcode::PopToA: {
                const Expected<uint64_t> tmp = Pop<uint64_t>();
                if (!tmp.HasValue()) ReturnFromBenchmark(false);
                a = tmp.Get();
                ReturnFromBenchmark(true);
            }
            default: ReturnFromBenchmark(false);
        }
    }
    String MathArch::ToString(const String& padding) const {
        StartBenchmark
        String ret = padding + "{\n";
        ret += padding + "\tPC = " + pc.ToString() + '\n';
        ret += padding + "\tSP = " + sp.ToString() + '\n';
        ret += padding + "\tA = " + a.ToString() + '\n';
        ReturnFromBenchmark(ret + padding + '}');
    }
}