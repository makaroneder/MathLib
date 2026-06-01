#ifdef __x86_64__
#include "SerialPortFirstInFirstOutControlRegister.hpp"
#include "SerialPortLineStatusRegister.hpp"
#include "SerialPort.hpp"
#include "../IO.hpp"

SerialPort::SerialPort(Port port) : port(port) {}
bool SerialPort::Init(void) {
    WritePort<uint8_t>((uint16_t)port + (uint16_t)IOOffset::InterruptEnableRegister, 0);
    SerialPortLineControlRegister reg;
    reg.divisorLatchAccess = true;
    SetLineControlRegister(reg);
    WritePort<uint8_t>((uint16_t)port + (uint16_t)IOOffset::LowerDivisor, 0x03);
    WritePort<uint8_t>((uint16_t)port + (uint16_t)IOOffset::UpperDivisor, 0x00);
    reg.divisorLatchAccess = false;
    SetLineControlRegister(reg);
    const SerialPortFirstInFirstOutControlRegister fifo;
    WritePort<uint8_t>((uint16_t)port + (uint16_t)IOOffset::FirstInFirstOutControlRegisters, *(const uint8_t*)&fifo);
    SerialPortModemControlRegister modemControl;
    SetModemControlRegister(modemControl);
    modemControl.dataTerminalReady = false;
    modemControl.out1 = modemControl.loop = true;
    SetModemControlRegister(modemControl);
    Write(0xaa);
    if (Read() != 0xaa) return false;
    modemControl.loop = false;
    SetModemControlRegister(modemControl);
    return true;
}
bool SerialPort::Skip(size_t size) {
    return DefaultSkip(size);
}
size_t SerialPort::ReadSizedBuffer(void* buffer, size_t size) {
    uint8_t* buff8 = (uint8_t*)buffer;
    for (size_t i = 0; i < size; i++) buff8[i] = Read();
    return size;
}
size_t SerialPort::WriteSizedBuffer(const void* buffer, size_t size) {
    const uint8_t* buff8 = (const uint8_t*)buffer;
    for (size_t i = 0; i < size; i++) Write(buff8[i]);
    return size;
}
bool SerialPort::CanRead(void) {
    const uint8_t tmp = ReadPort<uint8_t>((uint16_t)port + (uint16_t)IOOffset::LineStatusRegister);
    const SerialPortLineStatusRegister reg = *(const SerialPortLineStatusRegister*)&tmp;
    return !reg.dataReady;
}
bool SerialPort::CanWrite(void) {
    const uint8_t tmp = ReadPort<uint8_t>((uint16_t)port + (uint16_t)IOOffset::LineStatusRegister);
    const SerialPortLineStatusRegister reg = *(const SerialPortLineStatusRegister*)&tmp;
    return !reg.transmitterEmpty;
}
uint8_t SerialPort::Read(void) {
    while (CanRead()) {}
    return ReadPort<uint8_t>((uint16_t)port + (uint16_t)IOOffset::Buffer);
}
void SerialPort::Write(uint8_t value) {
    while (CanWrite()) {}
    WritePort<uint8_t>((uint16_t)port + (uint16_t)IOOffset::Buffer, value);
}
SerialPortLineControlRegister SerialPort::GetLineControlRegister(void) {
    const uint8_t tmp = ReadPort<uint8_t>((uint16_t)port + (uint16_t)IOOffset::LineControlRegister);
    return *(const SerialPortLineControlRegister*)&tmp;
}
void SerialPort::SetLineControlRegister(const SerialPortLineControlRegister& reg) {
    WritePort<uint8_t>((uint16_t)port + (uint16_t)IOOffset::LineControlRegister, *(const uint8_t*)&reg);
}
void SerialPort::SetModemControlRegister(const SerialPortModemControlRegister& reg) {
    WritePort<uint8_t>((uint16_t)port + (uint16_t)IOOffset::ModemControlRegister, *(const uint8_t*)&reg);
}

#endif