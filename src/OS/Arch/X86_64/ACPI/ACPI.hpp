#ifndef ACPI_H
#define ACPI_H
#include "RSDP.hpp"

bool InitACPI(const RSDP* rsdp, bool nmi);

#endif