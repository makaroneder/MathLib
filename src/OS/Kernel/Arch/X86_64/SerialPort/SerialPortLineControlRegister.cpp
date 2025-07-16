#ifdef __x86_64__
#include "SerialPortLineControlRegister.hpp"

SerialPortLineControlRegister::SerialPortLineControlRegister(void) : dataBits(0b11), stopBits(false), parity((uint8_t)Parity::None), breakEnable(false), divisorLatchAccess(false) {}
uint8_t SerialPortLineControlRegister::GetDataBits(void) const {
    return dataBits + 5;
}
SerialPortLineControlRegister::StopBits SerialPortLineControlRegister::GetStopBits(void) const {
    return stopBits ? (dataBits ? StopBits::Two : StopBits::OneAndHalf) : StopBits::One;
}

#endif