#ifdef __x86_64__
#include "GenericAddressStructure.hpp"
#include "../IO.hpp"

MathLib::Expected<uint64_t> GenericAddressStructure::Read(AccessSize legacySize) const {
    const AccessSize size = accessSize == AccessSize::Undefined ? legacySize : accessSize;
    switch (addressSpace) {
        case AddressSpace::SystemMemory: switch (size) {
            case AccessSize::U8Access: return MathLib::Expected<uint64_t>(*(uint8_t*)address);
            case AccessSize::U16Access: return MathLib::Expected<uint64_t>(*(uint16_t*)address);
            case AccessSize::U32Access: return MathLib::Expected<uint64_t>(*(uint32_t*)address);
            case AccessSize::U64Access: return MathLib::Expected<uint64_t>(*(uint64_t*)address);
            default: return MathLib::Expected<uint64_t>();
        }
        case AddressSpace::SystemIO: switch (size) {
            case AccessSize::U8Access: return MathLib::Expected<uint64_t>(ReadPort<uint8_t>(address));
            case AccessSize::U16Access: return MathLib::Expected<uint64_t>(ReadPort<uint16_t>(address));
            case AccessSize::U32Access: return MathLib::Expected<uint64_t>(ReadPort<uint32_t>(address));
            default: return MathLib::Expected<uint64_t>();
        }
        default: return MathLib::Expected<uint64_t>();
    }
}
bool GenericAddressStructure::Write(uint64_t value, AccessSize legacySize) {
    const AccessSize size = accessSize == AccessSize::Undefined ? legacySize : accessSize;
    switch (addressSpace) {
        case AddressSpace::SystemMemory: switch (size) {
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
        case AddressSpace::SystemIO: switch (size) {
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
        default: return false;
    }
}

#endif