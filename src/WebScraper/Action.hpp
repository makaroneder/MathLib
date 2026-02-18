#ifndef Action_H
#define Action_H
#include <Interfaces/Orderable.hpp>
#include <Interfaces/Printable.hpp>

struct Action : MathLib::Orderable, MathLib::Printable {
    enum class Type : uint8_t {
        Ignore,
        Scrape,
        Save,
        IgnoreException,
    };
    MathLib::String prefix;
    Type type;

    Action(void);
    Action(Type type, const MathLib::Sequence<char>& prefix);
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;

    protected:
    [[nodiscard]] virtual bool LessThanEqual(const MathLib::Orderable& other) const override;
};

#endif