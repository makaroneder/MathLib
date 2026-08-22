#ifndef ConstructorLogger_H
#define ConstructorLogger_H
#include "Interfaces/Orderable.hpp"
#include "Interfaces/Printable.hpp"

namespace MathLib {
    struct ConstructorLogger;
    struct ConstructorLogger : Orderable<ConstructorLogger>, Printable {
        static size_t currentID;

        ConstructorLogger(void);
        ConstructorLogger(const ConstructorLogger& other);
        ConstructorLogger(ConstructorLogger&& other);
        ~ConstructorLogger(void);
        ConstructorLogger& operator=(const ConstructorLogger& other);
        ConstructorLogger& operator=(ConstructorLogger&& other);
        [[nodiscard]] virtual bool LessThanEqual(const ConstructorLogger& other) const override;
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;

        private:
        size_t id;
    };
}

#endif