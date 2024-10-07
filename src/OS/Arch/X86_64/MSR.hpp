#ifndef MSR_H
#define MSR_H
#include <Expected.hpp>

enum class EFER : uint8_t {
    SystemCallExtensions = 0,
    LongModeEnable = 8,
    LongModeActive = 10,
    NoExecuteEnable,
    SecureVirtualMachineEnable,
    LongModeSegmentLimitEnable,
    FastFXSave,
    TranslationCacheExtension,
};
bool HasMSR(void);
Expected<uint64_t> GetMSR(uint32_t msr);
bool SetMSR(uint32_t msr, uint64_t value);

#endif