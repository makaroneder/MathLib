#ifdef __x86_64__
#ifndef LocalAPIC_H
#define LocalAPIC_H
#include <stdint.h>

// TODO: Custom struct for spurious interrupt vector, icr[0]
struct LVT {
    enum class Type : uint8_t {
        NMI = 0b100,
    };
    uint8_t vector;
    Type type : 3;
    uint8_t reserved1 : 1;
    bool pending : 1;
    bool lowTriggeredPolarity : 1;
    bool remoteIRR : 1;
    bool levelTriggered : 1;
    bool mask : 1;
    uint64_t reserved2;
    uint64_t reserved3 : 40;
} __attribute__((packed));
struct LocalAPICRegister {
    uint32_t value;
    uint64_t reserved1;
    uint32_t reserved2;
} __attribute__((packed));
struct LocalAPIC {
    LocalAPICRegister reserved1[2];
    LocalAPICRegister id;
    LocalAPICRegister version;
    LocalAPICRegister reserved2[4];
    LocalAPICRegister tpr;
    LocalAPICRegister apr;
    LocalAPICRegister ppr;
    LocalAPICRegister eoi;
    LocalAPICRegister rrd;
    LocalAPICRegister logicalDestination;
    LocalAPICRegister destinationFormat;
    LocalAPICRegister spuriousInterruptVector;
    LocalAPICRegister isr[8];
    LocalAPICRegister tmr[8];
    LocalAPICRegister irr[8];
    LocalAPICRegister errorStatus;
    LocalAPICRegister reserved3[6];
    LVT lvtCMCI;
    LocalAPICRegister icr[2];
    LVT lvtTimer;
    LVT lvtThermalSensor;
    LVT lvtPerformanceMonitoringCounters;
    LVT lvtLint[2];
    LVT lvtError;
    LocalAPICRegister initialTimerCount;
    LocalAPICRegister currentTimerCount;
    LocalAPICRegister reserved4[4];
    LocalAPICRegister timerDivideConfiguration;
    LocalAPICRegister reserved5;
} __attribute__((packed));

#endif
#endif