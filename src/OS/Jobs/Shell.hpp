#ifndef Shell_H
#define Shell_H
#include <Job/Job.hpp>
#include <Typedefs.hpp>

struct Shell : MathLib::Job {
    Shell(void);

    protected:
    [[nodiscard]] virtual MathLib::JobState RunInternal(void) override;

    private:
    MathLib::String command;
    bool init;
};

#endif