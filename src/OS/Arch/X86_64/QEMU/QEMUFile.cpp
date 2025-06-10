#ifdef __x86_64__
#include "QEMUFile.hpp"

QEMUFile::QEMUFile(void) : size(0), selector(QEMUSelector::Signature), free(true) {}
QEMUFile::QEMUFile(QEMUSelector selector, uint32_t size) : size(size), selector(selector), free(false) {}
bool QEMUFile::operator==(const QEMUFile& other) const {
    return selector == other.selector;
}
bool QEMUFile::operator!=(const QEMUFile& other) const {
    return !(*this == other);
}

#endif