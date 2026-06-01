#ifdef __x86_64__
#include "Exceptions/HypervisorInjectionException.hpp"
#include "Exceptions/ControlProtectionException.hpp"
#include "Exceptions/CoprocessorSegmentOverrun.hpp"
#include "Exceptions/VMMCommunicationException.hpp"
#include "Exceptions/VirtualizationException.hpp"
#include "Exceptions/GeneralProtectionFault.hpp"
#include "InterruptDescriptorTableRegister.hpp"
#include "Exceptions/NonMaskableInterrupt.hpp"
#include "Exceptions/DeviceNotAvailable.hpp"
#include "Exceptions/SIMDFloatException.hpp"
#include "Exceptions/StackSegmentFault.hpp"
#include "Exceptions/SegmentNotPresent.hpp"
#include "Exceptions/X87FloatException.hpp"
#include "Exceptions/SecurityException.hpp"
#include "Exceptions/BoundRangeError.hpp"
#include "Exceptions/DebugException.hpp"
#include "Exceptions/AlignmentCheck.hpp"
#include "Exceptions/DivisionError.hpp"
#include "Exceptions/InvalidOpcode.hpp"
#include "Exceptions/MachineCheck.hpp"
#include "Exceptions/DoubleFault.hpp"
#include "Exceptions/InvalidTSS.hpp"
#include "Exceptions/Breakpoint.hpp"
#include "Exceptions/PageFault.hpp"
#include "Exceptions/Overflow.hpp"
#include "SystemCallHandler.hpp"
#include "InterruptDevice.hpp"
#include "Interrupts.hpp"
#include "PIC8259.hpp"
#include "Cascade.hpp"
#include "../CPU.hpp"
#include <MainData.h>
#include <Host.hpp>

bool initialized = false;
size_t interruptDisabledCount = 1;
InterruptDescriptorTable idt;
InterruptDescriptorTableRegister idtr = InterruptDescriptorTableRegister(&idt);
PIC8259 pic;
extern "C" uintptr_t isrFunctionTable[];
InterruptDevice* interruptDevices[256];
DivisionError divisionError;
DebugException debugException;
NonMaskableInterrupt nonMaskableInterrupt;
Breakpoint breakpoint;
Overflow overflow;
BoundRangeError boundRangeError;
InvalidOpcode invalidOpcode;
DeviceNotAvailable deviceNotAvailable;
DoubleFault doubleFault;
CoprocessorSegmentOverrun coprocessorSegmentOverrun;
InvalidTSS invalidTSS;
SegmentNotPresent segmentNotPresent;
StackSegmentFault stackSegmentFault;
GeneralProtectionFault generalProtectionFault;
PageFault pageFault;
X87FloatException x87FloatException;
AlignmentCheck alignmentCheck;
MachineCheck machineCheck;
SIMDFloatException simdFloatException;
VirtualizationException virtualizationException;
ControlProtectionException controlProtectionException;
HypervisorInjectionException hypervisorInjectionException;
VMMCommunicationException vmmCommunicationException;
SecurityException securityException;
Cascade cascade;
SystemCallHandler systemCall;

struct Task {
    static constexpr size_t stackSize = 8 * 4096;
    enum class State : uint8_t {
        Init,
        Running,
        Dead,
    };
    Task(void);
    Task(uintptr_t address, const MainData* data);
    ~Task(void);
    [[nodiscard]] bool Save(const Registers& base);
    [[nodiscard]] bool Load(Registers& base);
    [[nodiscard]] bool Remove(void);
    [[nodiscard]] bool IsDead(void);
    [[nodiscard]] bool CanRun(void);
    
    private:
    void Init(const Registers& base);

    Registers regs;
    uint8_t* stack;
    State state;
};
Task::Task(void) : regs(), stack(nullptr), state() {}
Task::Task(uintptr_t address, const MainData* data) : regs(), stack(new uint8_t[stackSize + 15]), state(State::Init) {
    regs.ip = address;
    regs.di = (uintptr_t)data;
    regs.ksp = ((uintptr_t)stack + stackSize) / 16 * 16;
}
Task::~Task(void) {
    if (stack) delete [] stack;
}
void Task::Init(const Registers& base) {
    regs.cs = base.cs;
    regs.ds = base.ds;
    regs.es = base.es;
    regs.fs = base.fs;
    regs.gs = base.gs;
    regs.ss = base.ss;
    regs.kss = base.kss;
    regs.flags = 0x202;
    state = State::Running;
}
bool Task::Save(const Registers& base) {
    switch (state) {
        case State::Init: {
            Init(base);
            return true;
        }
        case State::Running: {
            regs = base;
            return true;
        }
        default: return false;
    }
}
bool Task::Load(Registers& base) {
    switch (state) {
        case State::Init: {
            Init(base);
            break;
        }
        case State::Running: break;
        default: return false;
    }
    base = regs;
    return true;
}
bool Task::Remove(void) {
    if (state == State::Dead) return false;
    state = State::Dead;
    return true;
}
bool Task::IsDead(void) {
    return state == State::Dead;
}
bool Task::CanRun(void) {
    return state != State::Dead;
}
void ArchSetInterrupts(bool value) {
    MathLib::X86Flags flags = GetFlags();
    if (value) {
        if (!--interruptDisabledCount) flags.interruptEnable = true;
    }
    else if (!interruptDisabledCount++) flags.interruptEnable = false;
    SetFlags(flags.value);
}

MathLib::Array<Task*> tasks;
size_t task = 0;

size_t ArchAddTask(uintptr_t address, const MainData* data) {
    ArchSetInterrupts(false);
    const size_t size = tasks.GetSize();
    Task* task = new Task(address, data);
    if (!task) return SIZE_MAX;
    size_t ret = SIZE_MAX;
    for (size_t i = 0; i < size; i++) {
        if (!tasks.AtUnsafe(i)->IsDead()) continue;
        delete tasks.AtUnsafe(i);
        tasks.AtUnsafe(i) = task;
        ret = i;
        break;
    }
    if (ret == SIZE_MAX) ret = tasks.Add(task) ? size : SIZE_MAX;
    ArchSetInterrupts(true);
    return ret;
}
bool ArchRemoveTask(size_t task) {
    ArchSetInterrupts(false);
    if (task >= tasks.GetSize()) return false;
    const bool ret = tasks.AtUnsafe(task)->Remove();
    ArchSetInterrupts(true);
    return ret;
}
size_t ArchGetCurrentTask(void) {
    return task;
}
size_t interruptCount = 0;
const size_t maxInterruptCount = 1000;
extern "C" void InterruptHandler(uintptr_t interrupt, Registers* regs, uintptr_t error) {
    if (interruptDevices[interrupt]) interruptDevices[interrupt]->OnInterrupt(interrupt, regs, error);
    if (tasks.GetSize() && ++interruptCount == maxInterruptCount) {
        interruptCount = 0;
        if (!tasks.AtUnsafe(task)->Save(*regs)) MathLib::Panic("Failed to save task context");
        const size_t prev = task;
        while (true) {
            task = (task + 1) % tasks.GetSize();
            if (tasks.AtUnsafe(task)->CanRun()) break;
            if (task == prev) MathLib::Panic("Every task is dead");
        }
        if (!tasks.AtUnsafe(task)->Load(*regs)) MathLib::Panic("Failed to load task context");
    }
    interrupt -= pic.GetBase();
    if (interrupt <= 15 && !pic.SendEndOfInterrupt(interrupt)) MathLib::Panic("Failed to send EOI to PIC");
}
uint8_t GetIRQBase(void) {
    return pic.GetBase();
}
void RegisterInterruptDevice(uintptr_t interrupt, InterruptDevice* device) {
    interruptDevices[interrupt] = device;
}
bool RegisterIRQDevice(IRQ irq, InterruptDevice* device) {
    const uintptr_t interrupt = pic.GetBase() + (uint8_t)irq;
    RegisterInterruptDevice(interrupt, device);
    pic.SetIRQMask(interrupt, !device);
    return !initialized || pic.UpdateMask();
}
bool InitInterrupts(uint8_t irqBase, uint8_t codeSegment) {
    if (initialized || !pic.Init(irqBase)) return false;
    for (uintptr_t i = 0; i < 256; i++)
        idt.descriptors[i] = InterruptDescriptor(isrFunctionTable[i], codeSegment, InterruptDescriptor::GateType::Interrupt);
    asm volatile("lidt %0" : : "m"(idtr));
    if (!AlignmentCheck::Enable() || !MachineCheck::Enable()) return false;
    ArchSetInterrupts(true);
    initialized = true;
    return true;
}

#endif