#ifndef DateKeeper_H
#define DateKeeper_H
#include <Date.hpp>

struct DateKeeper {
    [[nodiscard]] virtual MathLib::Date GetDate(void) const = 0;
    virtual void SetDate(const MathLib::Date& date) = 0;
};
extern DateKeeper* dateKeeper;

#endif