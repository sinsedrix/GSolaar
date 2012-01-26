#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([1.9])
AC_INIT([GSolaar], [0.4.2], [sinsedrix@users.sourceforge.net], [GSolaar])
AC_CONFIG_SRCDIR([src/gsolaar.c])
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE([GSolaar], [0.4.2])
AM_GNU_GETTEXT([external])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C_O
AC_PROG_RANLIB
AC_PROG_INSTALL

# Check for pkg-config
PKG_PROG_PKG_CONFIG(0.16)

# Check for windres
AC_CHECK_TOOL(WINDRES, windres, :)
AM_CONDITIONAL(HAVE_WINDRES, test "x$WINDRES" != "x:")

# Check for NSIS
AC_CHECK_TOOL(NSIS, makensis, :)
AM_CONDITIONAL(HAVE_NSIS, test "x$NSIS" != "x:")


AC_CHECK_HEADERS([math.h stdio.h]) #stdlib.h  string.h unistd.h 
AC_CHECK_FUNCS([floor sqrt strdup putenv setenv])
 
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRTOD

CFLAGS="-Wall -g -O0"
 
# Check for GTK+2 (already checked in GtkGLExt check
# AM_PATH_GTK_2_0([2.12.5], :, AC_MSG_ERROR(Test for GTK+ failed. See the file 'INSTALL' for help.), gthread)

# Check for GTKGlExt
AM_PATH_GTKGLEXT_1_0([1.0.0], :, AC_MSG_ERROR(Test for GTKGlExt failed. See the file 'INSTALL' for help.))
CFLAGS="$CFLAGS $GTKGLEXT_CFLAGS"
LIBS="$LIBS $GTKGLEXT_LIBS"

# Autoconf classique
AC_OUTPUT(Makefile src/Makefile src/kaleido/Makefile src/core/Makefile src/export/Makefile src/widgets/Makefile src/share/images/Makefile po/Makefile.in)

# Autoconf de dev avec tests unitaires
#AC_OUTPUT(Makefile src/Makefile src/kaleido/Makefile src/core/Makefile src/export/Makefile src/widgets/Makefile src/share/images/Makefile po/Makefile.in tu/core/Makefile tu/widgets/Makefile)

# Autoconf de livraison
#AC_OUTPUT(Makefile src/Makefile src/kaleido/Makefile src/core/Makefile src/export/Makefile src/widgets/Makefile src/share/images/Makefile po/Makefile.in packaging/win32/Makefile packaging/rpm/Makefile)
