#include "TLSRecordHeader.hpp"

TLSRecordHeader::TLSRecordHeader(void) : type(), version(0), size(0) {}
TLSRecordHeader::TLSRecordHeader(Type type, uint16_t version, uint16_t size) : type(type), version(version), size(size) {}