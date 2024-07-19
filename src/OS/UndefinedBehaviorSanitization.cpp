#include <Host.hpp>

struct SourceLocation {
    const char* file;
    uint32_t line;
    uint32_t column;
};
String SourceLocationToString(SourceLocation* location) {
    return String(location->file) + " " + ToString(location->line) + ":" + ToString(location->column);
}
extern "C" void __ubsan_handle_type_mismatch(SourceLocation* info, uintptr_t ptr) {
    // TODO:
    (void)ptr;
    Panic(String(__func__) + " " + SourceLocationToString(info));
}
extern "C" void __ubsan_handle_out_of_bounds(SourceLocation* info) {
    // TODO:
    Panic(String(__func__) + " " + SourceLocationToString(info));
}
extern "C" void __ubsan_handle_pointer_overflow(SourceLocation* info) {
    // TODO:
    Panic(String(__func__) + " " + SourceLocationToString(info));
}
extern "C" void __ubsan_handle_type_mismatch_v1(SourceLocation* info) {
    // TODO:
    Panic(String(__func__) + " " + SourceLocationToString(info));
}
extern "C" void __ubsan_handle_sub_overflow(SourceLocation* info) {
    // TODO:
    Panic(String(__func__) + " " + SourceLocationToString(info));
}
extern "C" void __ubsan_handle_add_overflow(SourceLocation* info) {
    // TODO:
    Panic(String(__func__) + " " + SourceLocationToString(info));
}
extern "C" void __ubsan_handle_mul_overflow(SourceLocation* info) {
    // TODO:
    Panic(String(__func__) + " " + SourceLocationToString(info));
}
extern "C" void __ubsan_handle_divrem_overflow(SourceLocation* info) {
    // TODO:
    Panic(String(__func__) + " " + SourceLocationToString(info));
}
extern "C" void __ubsan_handle_load_invalid_value(SourceLocation* info) {
    // TODO:
    Panic(String(__func__) + " " + SourceLocationToString(info));
}
extern "C" void __ubsan_handle_shift_out_of_bounds(SourceLocation* info) {
    // TODO:
    Panic(String(__func__) + " " + SourceLocationToString(info));
}
extern "C" void __ubsan_handle_vla_bound_not_positive(SourceLocation* info) {
    // TODO:
    Panic(String(__func__) + " " + SourceLocationToString(info));
}
extern "C" void __ubsan_handle_negate_overflow(SourceLocation* info) {
    // TODO:
    Panic(String(__func__) + " " + SourceLocationToString(info));
}