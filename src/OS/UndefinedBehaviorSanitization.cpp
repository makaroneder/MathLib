#include <Host.hpp>
#include <String.hpp>

struct SourceLocation {
    const char* file;
    uint32_t line;
    uint32_t column;
};
struct TypeDescriptor {
    uint16_t type;
    uint16_t info;
    char name[];
};
template <typename T>
struct TypeMismatchInfo : SourceLocation {
    enum class Type : uint8_t {
        Load,
        Store,
        ReferenceBinding,
        MemberAccess,
        MemberCall,
        ConstructorCall,
        Downcast1,
        Downcast2,
        Upcast,
        CastToVirtualBase,
    };
    TypeDescriptor* typeDescriptor;
    T alignment;
    Type type;
};

[[nodiscard]] MathLib::String SourceLocationToString(SourceLocation* location) {
    return MathLib::String(location->file) + " " + MathLib::ToString(location->line) + ":" + MathLib::ToString(location->column);
}
extern "C" void __ubsan_handle_type_mismatch(TypeMismatchInfo<uintptr_t>* info, uintptr_t ptr) {
    MathLib::String tmp;
    if (!ptr) tmp = "Null pointer access";
    else if (info->alignment && !(ptr & (info->alignment - 1))) tmp = "Unaligned memory access";
    else {
        switch (info->type) {
            case TypeMismatchInfo<uintptr_t>::Type::Load: {
                tmp = "Load of";
                break;
            }
            case TypeMismatchInfo<uintptr_t>::Type::Store: {
                tmp = "Store to";
                break;
            }
            case TypeMismatchInfo<uintptr_t>::Type::ReferenceBinding: {
                tmp = "Reference binding to";
                break;
            }
            case TypeMismatchInfo<uintptr_t>::Type::MemberAccess: {
                tmp = "Member access in";
                break;
            }
            case TypeMismatchInfo<uintptr_t>::Type::MemberCall: {
                tmp = "Member call on";
                break;
            }
            case TypeMismatchInfo<uintptr_t>::Type::ConstructorCall: {
                tmp = "Constructor call on";
                break;
            }
            case TypeMismatchInfo<uintptr_t>::Type::Downcast1:
            case TypeMismatchInfo<uintptr_t>::Type::Downcast2: {
                tmp = "Downcast of";
                break;
            }
            case TypeMismatchInfo<uintptr_t>::Type::Upcast: {
                tmp = "Upcast of";
                break;
            }
            case TypeMismatchInfo<uintptr_t>::Type::CastToVirtualBase: {
                tmp = "Cast to virtual base of";
                break;
            }
        }
        tmp += " 0x"_M + MathLib::ToString(ptr, 16) + " with not enough space for " + info->typeDescriptor->name;
    }
    MathLib::Panic(MathLib::String(__func__) + ' ' + SourceLocationToString(info) + ' ' + tmp + '\n');
}
extern "C" void __ubsan_handle_out_of_bounds(SourceLocation* info) {
    // TODO:
    MathLib::Panic(MathLib::String(__func__) + " " + SourceLocationToString(info));
}
extern "C" void __ubsan_handle_pointer_overflow(SourceLocation* info) {
    // TODO:
    MathLib::Panic(MathLib::String(__func__) + " " + SourceLocationToString(info));
}
extern "C" void __ubsan_handle_type_mismatch_v1(TypeMismatchInfo<uint8_t>* info, uintptr_t ptr) {
    MathLib::String tmp;
    if (!ptr) tmp = "Null pointer access";
    else if (info->alignment && !(ptr & (1 << info->alignment))) tmp = "Unaligned memory access";
    else {
        switch (info->type) {
            case TypeMismatchInfo<uint8_t>::Type::Load: {
                tmp = "Load of";
                break;
            }
            case TypeMismatchInfo<uint8_t>::Type::Store: {
                tmp = "Store to";
                break;
            }
            case TypeMismatchInfo<uint8_t>::Type::ReferenceBinding: {
                tmp = "Reference binding to";
                break;
            }
            case TypeMismatchInfo<uint8_t>::Type::MemberAccess: {
                tmp = "Member access in";
                break;
            }
            case TypeMismatchInfo<uint8_t>::Type::MemberCall: {
                tmp = "Member call on";
                break;
            }
            case TypeMismatchInfo<uint8_t>::Type::ConstructorCall: {
                tmp = "Constructor call on";
                break;
            }
            case TypeMismatchInfo<uint8_t>::Type::Downcast1:
            case TypeMismatchInfo<uint8_t>::Type::Downcast2: {
                tmp = "Downcast of";
                break;
            }
            case TypeMismatchInfo<uint8_t>::Type::Upcast: {
                tmp = "Upcast of";
                break;
            }
            case TypeMismatchInfo<uint8_t>::Type::CastToVirtualBase: {
                tmp = "Cast to virtual base of";
                break;
            }
        }
        tmp += " 0x"_M + MathLib::ToString(ptr, 16) + " with not enough space for " + info->typeDescriptor->name;
    }
    MathLib::Panic(MathLib::String(__func__) + ' ' + SourceLocationToString(info) + ' ' + tmp + '\n');
}
extern "C" void __ubsan_handle_sub_overflow(SourceLocation* info) {
    // TODO:
    MathLib::Panic(MathLib::String(__func__) + " " + SourceLocationToString(info));
}
extern "C" void __ubsan_handle_add_overflow(SourceLocation* info) {
    // TODO:
    MathLib::Panic(MathLib::String(__func__) + " " + SourceLocationToString(info));
}
extern "C" void __ubsan_handle_mul_overflow(SourceLocation* info) {
    // TODO:
    MathLib::Panic(MathLib::String(__func__) + " " + SourceLocationToString(info));
}
extern "C" void __ubsan_handle_divrem_overflow(SourceLocation* info) {
    // TODO:
    MathLib::Panic(MathLib::String(__func__) + " " + SourceLocationToString(info));
}
extern "C" void __ubsan_handle_load_invalid_value(SourceLocation* info) {
    // TODO:
    MathLib::Panic(MathLib::String(__func__) + " " + SourceLocationToString(info));
}
extern "C" void __ubsan_handle_shift_out_of_bounds(SourceLocation* info) {
    // TODO:
    MathLib::Panic(MathLib::String(__func__) + " " + SourceLocationToString(info));
}
extern "C" void __ubsan_handle_vla_bound_not_positive(SourceLocation* info) {
    // TODO:
    MathLib::Panic(MathLib::String(__func__) + " " + SourceLocationToString(info));
}
extern "C" void __ubsan_handle_negate_overflow(SourceLocation* info) {
    // TODO:
    MathLib::Panic(MathLib::String(__func__) + " " + SourceLocationToString(info));
}