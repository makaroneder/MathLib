#ifdef __x86_64__
#ifndef ACPI_H
#define ACPI_H
#include "RSDP.hpp"

[[nodiscard]] bool InitACPI(const RSDP* rsdp, bool nmi);

#endif
#endif