AC_PREREQ([2.68])
AC_INIT([largenet], [0.4])

AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_HEADERS([config.h])

# Global version substitutions
AC_SUBST([LARGENET_SO_VERSION], [0:4])
AC_SUBST([LARGENET_API_VERSION], [0.4])

# Checks for programs.
AC_PROG_CXX

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([floor pow sqrt])

AC_CONFIG_FILES([Makefile])

AM_INIT_AUTOMAKE([1.10 no-define])
LT_INIT([disable-static])

AC_OUTPUT