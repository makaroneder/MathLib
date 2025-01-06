#ifdef __x86_64__
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
[[nodiscard]] bool HasMSR(void);
[[nodiscard]] MathLib::Expected<uint64_t> GetMSR(uint32_t msr);
[[nodiscard]] bool SetMSR(uint32_t msr, uint64_t value);

#endif
#endif