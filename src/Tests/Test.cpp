#include "Test.hpp"

bool Test::ReportRecord(const Record& record) {
    return records.Add(record);
}
size_t Test::GetRecordCount(void) const {
    return records.GetSize();
}
size_t Test::GetPassed(void) const {
    size_t ret = 0;
    for (const Record& record : records) ret += record.passed;
    return ret;
}
String Test::ToString(const String& padding) const {
    String ret;
    for (const Record& record : records) ret += record.ToString(padding) + '\n';
    return ret;
}