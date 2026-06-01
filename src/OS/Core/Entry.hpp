#ifndef Entry_H
#define Entry_H

struct RSDP;
[[noreturn]] void Main(void);
[[nodiscard]] bool InitBootloader(RSDP*& rsdp);

#endif