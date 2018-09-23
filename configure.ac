AC_PREREQ([2.64])
AC_CONFIG_AUX_DIR([build-aux])

AC_INIT([anq], [0.0.1], [contact@thecriw.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([config.h.in])

AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

AM_PATH_GPGME([1.11.1], [], [
	AC_MSG_ERROR(unable to find GPGME library.)
])

AC_CHECK_FUNCS([setlocale])

AC_CHECK_HEADERS([locale.h])
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([unistd.h])
AC_CHECK_HEADERS([termios.h])
AC_CHECK_HEADER_STDBOOL

AC_OUTPUT
