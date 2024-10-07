#ifndef DateKeeper_H
#define DateKeeper_H
#include "Date.hpp"

struct DateKeeper {
    virtual Date GetDate(void) const = 0;
    virtual void SetDate(const Date& date) = 0;
};
extern DateKeeper* dateKeeper;

#endif