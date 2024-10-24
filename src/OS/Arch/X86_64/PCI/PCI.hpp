#ifdef __x86_64__
#ifndef PCI_H
#define PCI_H
#include "../ACPI/MCFG.hpp"

// TODO: Add PCI header type 1 and 2
bool InitPCI(const MCFG* mcfg);

#endif
#endif