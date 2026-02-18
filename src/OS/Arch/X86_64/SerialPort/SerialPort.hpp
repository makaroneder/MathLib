#ifdef __x86_64__
#ifndef SerialPort_H
#define SerialPort_H
#include "SerialPortModemControlRegister.hpp"
#include "SerialPortLineControlRegister.hpp"
#include <Interfaces/Readable.hpp>
#include <Interfaces/Writable.hpp>

struct SerialPort : MathLib::Readable, MathLib::Writable {
    enum class Port : uint16_t {
        COM1 = 0x03f8,
        COM2 = 0x02f8,
        COM3 = 0x03e8,
        COM4 = 0x02e8,
        COM5 = 0x05f8,
        COM6 = 0x04f8,
        COM7 = 0x05e8,
        COM8 = 0x04e8,
    };
    enum class IOOffset : uint8_t {
        Buffer = 0,
        LowerDivisor = Buffer,
        InterruptEnableRegister,
        UpperDivisor = InterruptEnableRegister,
        InterruptIdentification,
        FirstInFirstOutControlRegisters = InterruptIdentification,
        LineControlRegister,
        ModemControlRegister,
        LineStatusRegister,
        ModemStatusRegister,
        ScratchRegister,
    };
    SerialPort(Port port);
    [[nodiscard]] bool Init(void);
    [[nodiscard]] virtual bool Skip(size_t size) override;
    [[nodiscard]] virtual size_t ReadSizedBuffer(void* buffer, size_t size) override;
    [[nodiscard]] virtual size_t WriteSizedBuffer(const void* buffer, size_t size) override;

    private:
    Port port;

    [[nodiscard]] uint8_t Read(void);
    void Write(uint8_t value);
    [[nodiscard]] bool CanRead(void);
    [[nodiscard]] bool CanWrite(void);
    [[nodiscard]] SerialPortLineControlRegister GetLineControlRegister(void);
    void SetLineControlRegister(const SerialPortLineControlRegister& reg);
    void SetModemControlRegister(const SerialPortModemControlRegister& reg);
};

#endif
#endif