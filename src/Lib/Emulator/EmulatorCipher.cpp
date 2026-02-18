#include "EmulatorCipher.hpp"

namespace MathLib {
    EmulatorCipher::EmulatorCipher(Emulator& emulator) : Emulator(), emulator(emulator) {}
    bool EmulatorCipher::Reset(void) {
        return true;
    }
    bool EmulatorCipher::Run(void) {
        return emulator.Reset() && emulator.Run();
    }
    Array<uint8_t> EmulatorCipher::Encrypt(const Sequence<uint8_t>& data, const CipherKey& key) const {
        if (!key.IsEmpty() || !emulator.Reset()) return Array<uint8_t>();
        emulator.memory = CollectionToArray<uint8_t>(data);
        return emulator.Run() ? emulator.memory : Array<uint8_t>();
    }
}