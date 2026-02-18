#ifdef __x86_64__
#include "SerialPortModemControlRegister.hpp"

SerialPortModemControlRegister::SerialPortModemControlRegister(void) : dataTerminalReady(true), requestToSend(true), out1(false), out2(true), loop(false), reserved(0) {}

#endif