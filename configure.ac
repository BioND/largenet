AC_PREREQ([2.67])
AC_INIT([largenet], [0.5])
AM_INIT_AUTOMAKE([foreign silent-rules])
AM_SILENT_RULES
LT_PREREQ([2.2])
LT_INIT([disable-static])

AC_CONFIG_MACRO_DIR([m4])

AC_LANG([C++])

# Checks for programs.
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_PROG_INSTALL

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.
AC_CHECK_FUNCS([floor pow sqrt])

# Sanity check
AC_CONFIG_SRCDIR([src/largenet/network/Network.h])

# Output files
AC_CONFIG_FILES([Makefile]
                [src/Makefile]
                [largenet-0.5.pc:largenet.pc.in])

AC_CONFIG_HEADERS([config.h])

AC_OUTPUT