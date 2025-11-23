#ifdef __x86_64__
#ifndef PCI_H
#define PCI_H
#include "../ACPI/MCFG.hpp"

[[nodiscard]] bool InitPCI(const MCFG* mcfg);

#endif
#endif