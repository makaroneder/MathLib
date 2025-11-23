#ifdef __x86_64__
#ifndef QEMUFile_H
#define QEMUFile_H
#include "QEMUSelector.hpp"

struct QEMUFile {
    uint32_t size;
    QEMUSelector selector;
    bool free;

    QEMUFile(void);
    QEMUFile(QEMUSelector selector, uint32_t size);
    [[nodiscard]] bool operator==(const QEMUFile& other) const;
    [[nodiscard]] bool operator!=(const QEMUFile& other) const;
};

#endif
#endif