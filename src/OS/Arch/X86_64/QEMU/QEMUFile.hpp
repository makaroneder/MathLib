#ifdef __x86_64__
#ifndef QEMUFile_H
#define QEMUFile_H
#include "QEMUSelector.hpp"
#include <Interfaces/Comparable.hpp>

struct QEMUFile;
struct QEMUFile : MathLib::Comparable<QEMUFile> {
    uint32_t size;
    QEMUSelector selector;
    bool free;

    QEMUFile(void);
    QEMUFile(QEMUSelector selector, uint32_t size);

    protected:
    [[nodiscard]] virtual bool Equals(const QEMUFile& other) const override;
};

#endif
#endif