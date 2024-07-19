#include "VM.hpp"
#include "IO.hpp"

void ShutdownVM(void) {
    // Bochs shutdown
    WritePort<uint16_t>(0xB004, 0x2000);
    // QEMU shutdown
    WritePort<uint16_t>(0x0604, 0x2000);
    // VirtualBox shutdown
    WritePort<uint16_t>(0x4004, 0x3400);
    // Cloud Hypervisor shutdown
    WritePort<uint16_t>(0x0600, 0x0034);
}