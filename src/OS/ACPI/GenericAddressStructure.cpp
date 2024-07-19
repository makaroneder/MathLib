#include "GenericAddressStructure.hpp"
#include "../IO.hpp"

uint64_t GenericAddressStructure::Read(void) const {
    switch (addressSpace) {
        case AddressSpace::SystemMemory: {
            switch (accessSize) {
                case AccessSize::U8Access: return *(uint8_t*)address;
                case AccessSize::U16Access: return *(uint16_t*)address;
                case AccessSize::U32Access: return *(uint32_t*)address;
                case AccessSize::U64Access: return *(uint64_t*)address;
                default: return 0;
            }
        }
        case AddressSpace::SystemIO: {
            switch (accessSize) {
                case AccessSize::U8Access: return ReadPort<uint8_t>(address);
                case AccessSize::U16Access: return ReadPort<uint16_t>(address);
                case AccessSize::U32Access: return ReadPort<uint32_t>(address);
                default: return 0;
            }
        }
        default: return 0;
    }
}
bool GenericAddressStructure::Write(uint64_t value) {
    switch (addressSpace) {
        case AddressSpace::SystemMemory: {
            switch (accessSize) {
                case AccessSize::U8Access: {
                    *(uint8_t*)address = value;
                    return true;
                }
                case AccessSize::U16Access: {
                    *(uint16_t*)address = value;
                    return true;
                }
                case AccessSize::U32Access: {
                    *(uint32_t*)address = value;
                    return true;
                }
                case AccessSize::U64Access: {
                    *(uint64_t*)address = value;
                    return true;
                }
                default: return false;
            }
        }
        case AddressSpace::SystemIO: {
            switch (accessSize) {
                case AccessSize::U8Access: {
                    WritePort<uint8_t>(address, value);
                    return true;
                }
                case AccessSize::U16Access: {
                    WritePort<uint16_t>(address, value);
                    return true;
                }
                case AccessSize::U32Access: {
                    WritePort<uint32_t>(address, value);
                    return true;
                }
                default: return false;
            }
        }
        default: return false;
    }
}