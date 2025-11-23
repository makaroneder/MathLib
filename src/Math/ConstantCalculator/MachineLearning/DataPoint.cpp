#include "DataPoint.hpp"

DataPoint::DataPoint(void) : input(), output() {}
DataPoint::DataPoint(const Formula& input, const Formula& output) : input(input), output(output) {}
bool DataPoint::Equals(const DataPoint& other) const {
    return input == other.input && output == other.output;
}