AC_PREREQ([2.65])
AC_INIT([largenet], [0.5])
AM_INIT_AUTOMAKE([1.11 -Wall -Werror foreign silent-rules subdir-objects])
LT_PREREQ([2.2])
LT_INIT([disable-static])

AC_CONFIG_MACRO_DIR([m4])
ACLOCAL_AMFLAGS = -I ./m4

AC_LANG([C++])

# Checks for programs.
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_TRY_DOXYGEN

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.
AC_CHECK_FUNCS([floor pow sqrt])

BOOST_REQUIRE([1.36.0])
BOOST_PROGRAM_OPTIONS

# Sanity check
AC_CONFIG_SRCDIR([largenet/base/TypedNetwork.h])

# provide --enable-examples option
AC_ARG_ENABLE([examples],
		      [AS_HELP_STRING([--enable-examples],
			  				  [build examples (default: no)])],
     		  [case "${enableval}" in
				yes) examples=true ;;
				no)  examples=false ;;
				*) AC_MSG_ERROR([bad value ${enableval} for --enable-examples]) ;;
     		   esac],
     		   [examples=false])
AM_CONDITIONAL([BUILD_EXAMPLES], [test x$examples = xtrue])

AM_CONDITIONAL([HAVE_DOXYGEN], [test -n "$DOXYGEN"])

# Output files
AC_CONFIG_FILES([Makefile
                largenet-0.5.pc:largenet.pc.in
				Doxyfile])

AC_CONFIG_HEADERS([config.h])

AC_OUTPUT
