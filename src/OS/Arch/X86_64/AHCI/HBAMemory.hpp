#ifndef HBAMemory_H
#define HBAMemory_H
#include "HBAPort.hpp"

struct HBAMemory {
    uint32_t capabilities;
    uint32_t globalHostControl;
    uint32_t interruptStatus;
    uint32_t portsImplemented;
    uint32_t version;
    uint32_t cccControl;
    uint32_t cccPorts;
    uint32_t enclosureManagementLocation;
    uint32_t enclosureManagementControl;
    uint32_t extendedCapabilities;
    uint32_t biosHandoffControlStatus;
    uint32_t reserved[29];
    uint64_t vendor[12];
    HBAPort ports[1];
};

#endif