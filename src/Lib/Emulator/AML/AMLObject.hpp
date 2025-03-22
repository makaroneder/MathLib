#ifndef MathLib_Emulator_AML_Object_H
#define MathLib_Emulator_AML_Object_H
#include "../../Interfaces/Printable.hpp"

namespace MathLib {
    struct AMLObject : Printable {
        enum class Type {
            Normal,
            OperationRegion,
            Field,
        };
        AMLObject(void);
        AMLObject(const Sequence<char>& name);
        AMLObject(const Sequence<char>& name, const Sequence<uint64_t>& data);
        AMLObject(const Sequence<char>& name, const Sequence<uint64_t>& data, Type type);
        [[nodiscard]] Array<uint64_t> GetData(void) const;
        [[nodiscard]] bool AddChild(const AMLObject& child);
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;

        private:
        Type type;
        String name;
        Array<uint64_t> data;
        Array<AMLObject> children;
    };
}

#endif