#ifdef __x86_64__
#ifndef CMOS_H
#define CMOS_H
#include <Interfaces/Printable.hpp>

struct CMOSFloppyData : MathLib::Printable {
    uint8_t data;

    CMOSFloppyData(uint8_t data);
    [[nodiscard]] uint8_t GetMaster(void) const;
    [[nodiscard]] uint8_t GetSlave(void) const;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
};
struct CMOS {
    enum class Register : uint8_t {
        RTCSeconds = 0x00,
        RTCMinutes = 0x02,
        RTCHours = 0x04,
        RTCWeekday = 0x06,
        RTCDay,
        RTCMonth,
        RTCYear,
        StatusRegisterA,
        StatusRegisterB,
        StatusRegisterC,
        FloppyData = 0x10,
    };
    CMOS(bool nmi_);
    CMOSFloppyData GetFloppyData(void) const;
    void SetNMI(bool nmi_);

    protected:
    [[nodiscard]] uint8_t Read(Register reg) const;
    void Write(Register reg, uint8_t value);

    private:
    void AccessRegister(Register reg) const;
    bool nmi;
};
extern CMOS* cmos;

#endif
#endif