#include "EmulatorCipher.hpp"

namespace MathLib {
    EmulatorCipher::EmulatorCipher(Emulator& emulator) : Emulator(), emulator(emulator) {
        EmptyBenchmark
    }
    void EmulatorCipher::Reset(void) {
        EmptyBenchmark
    }
    bool EmulatorCipher::Run(void) {
        StartBenchmark
        emulator.Reset();
        ReturnFromBenchmark(emulator.Run());
    }
    Array<uint8_t> EmulatorCipher::Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const {
        StartBenchmark
        if (!key.IsEmpty()) ReturnFromBenchmark(Array<uint8_t>());
        emulator.Reset();
        emulator.memory = CollectionToArray<uint8_t>(data);
        ReturnFromBenchmark(emulator.Run() ? emulator.memory : Array<uint8_t>());
    }
}