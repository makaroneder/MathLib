#ifndef DataPoint_H
#define DataPoint_H
#include "Formula.hpp"

struct DataPoint;
struct DataPoint : MathLib::Comparable<DataPoint> {
    Formula input;
    Formula output;

    DataPoint(void);
    DataPoint(const Formula& input, const Formula& output);

    protected:
    [[nodiscard]] virtual bool Equals(const DataPoint& other) const override;
};

#endif