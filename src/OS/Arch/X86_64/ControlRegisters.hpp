#ifdef __x86_64__
#ifndef ControlRegisters_H
#define ControlRegisters_H
#include <Expected.hpp>

enum class ControlRegister0 : uint8_t {
    ProtectedModeEnable = 0,
    MonitorCoProcessor,
    Emulation,
    TaskSwitched,
    ExtensionType,
    NumericError,
    WriteProtect = 16,
    AlignmentMask = 18,
    NotWriteThrough = 29,
    CacheDisable,
    Paging,
};
enum class ControlRegister4 : uint8_t {
    Virtual8086ModeExtensions = 0,
    ProtectedModeVirtualInterrupts,
    TimeStampOnlyInRing0,
    DebuggingExtensions,
    PageSizeExtension,
    PhysicalAddressExtension,
    MachineCheckException,
    PageGlobalEnable,
    PerformanceMonitoringCounterEnable,
    FXSaveOSSupport,
    UnmaskedSIMDFloatExceptionsOSSupport,
    UserModeInstructionPrevention,
    VirtualMachineExtensionsEnable = 13,
    SaferModeExtensionsEnable,
    EnableFSAndGSBase = 16,
    PCIDEnable,
    XSaveEnable,
    SupervisorModeExecutionsProtectionEnable = 20,
    SupervisorModeAccessProtectionEnable,
    EnableUserModePagesProtectionKeys,
    EnableControlFlowEnforcementTechnology,
    EnableSupervisorModePagesProtectionKeys,
};
MathLib::Expected<uintptr_t> GetControlRegister(uint8_t num);
bool SetControlRegister(uint8_t num, uintptr_t value);

#endif
#endif