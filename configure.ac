#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([cryptopals], [1.0], [evan@eklitzke.org])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_SRCDIR([src/main.cc])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_MACRO_DIR([m4])

AC_LANG([C++])
AM_INIT_AUTOMAKE([dist-bzip2 foreign subdir-objects -Wall -Werror])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([memmove memset strtoul])

AS_COMPILER_FLAG([-std=c++17], [AX_APPEND_FLAG([-std=c++17])])
AS_COMPILER_FLAG([-fdiagnostics-color=auto], [AX_APPEND_FLAG([-fdiagnostics-color=auto])])
AS_COMPILER_FLAG([-Wall], [AX_APPEND_FLAG([-Wall])])

# Disable CBC/CTR code from tiny-aes
AX_APPEND_FLAG([-DCBC=0])
AX_APPEND_FLAG([-DCTR=0])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
