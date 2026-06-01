#undef TARGET_MAKARON
#define TARGET_MAKARON 1

#undef STARTFILE_SPEC
#define STARTFILE_SPEC "crt0.o%s crti.o%s %{!shared: crtbegin.o%s} %{shared: crtbeginS.o%s}"

#undef ENDFILE_SPEC
#define ENDFILE_SPEC "%{!shared: crtend.o%s} %{shared: crtendS.o%s} crtn.o%s"

#undef STANDARD_STARTFILE_PREFIX
#define STANDARD_STARTFILE_PREFIX "/lib/"

#undef LIB_SPEC
#define LIB_SPEC "-lc"

#undef LINK_SPEC
#define LINK_SPEC "%{shared:-shared} %{static:-static} %{!shared: %{!static: %{rdynamic:-export-dynamic}}}"

#undef TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS() do {   \
    builtin_define ("__makaron__");     \
    builtin_define ("__unix__");        \
    builtin_assert ("system=makaron");  \
    builtin_assert ("system=unix");     \
    builtin_assert ("system=posix");    \
} while(0);
