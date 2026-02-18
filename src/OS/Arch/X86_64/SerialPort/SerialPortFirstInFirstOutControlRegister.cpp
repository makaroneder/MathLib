#ifdef __x86_64__
#include "SerialPortFirstInFirstOutControlRegister.hpp"

SerialPortFirstInFirstOutControlRegister::SerialPortFirstInFirstOutControlRegister(void) : enable(true), clearReceive(true), clearTransmit(true), dmaModeSelect(false), reserved(0), interruptTriggerLevel((uint8_t)InterruptTriggerLevel::Fourteen) {}

#endif