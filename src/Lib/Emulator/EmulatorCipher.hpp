#ifndef MathLib_Emulator_Cipher_H
#define MathLib_Emulator_Cipher_H
#include "Emulator.hpp"
#include "../Cryptography/OneWayCipher.hpp"

namespace MathLib {
    struct EmulatorCipher : Emulator, OneWayCipher {
        EmulatorCipher(Emulator& emulator);
        virtual void Reset(void) override;
        [[nodiscard]] virtual bool Run(void) override;
        [[nodiscard]] virtual Array<uint8_t> Encrypt(const Sequence<uint8_t>& data, const Sequence<uint64_t>& key) const override;

        private:
        Emulator& emulator;
    };
}

#endif