#ifndef MathLib_MAC_H
#define MathLib_MAC_H
#include "Interfaces/Saveable.hpp"

namespace MathLib {
    struct MAC : Collection<uint8_t>, Saveable, Printable {
        [[nodiscard]] virtual size_t GetSize(void) const override;
        [[nodiscard]] virtual bool Add(const uint8_t& value) override;
        [[nodiscard]] virtual bool Reset(void) override;
        [[nodiscard]] virtual uint8_t* GetValue(void) override;
        [[nodiscard]] virtual const uint8_t* GetValue(void) const override;
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;

        private:
        union {
            struct {
                bool multicast : 1;
                bool locallyAdministered : 1;
                uint8_t organisationallyUniqueIdentifier0 : 6;
                uint8_t organisationallyUniqueIdentifier[2];
                uint8_t networkInterfaceControllerSpecific[3];
            } __attribute__((packed));
            uint8_t buffer[6];
        };
    };
}

#endif