dnl Initialise autoconf

AC_INIT([hades], [0.0.0], [alriddoch@googlemail.com])
AC_CONFIG_SRCDIR([src/hades.cpp])
AC_CONFIG_HEADERS([config.h])

dnl Detect the canonical host and target environment

AC_CANONICAL_HOST

dnl Initialise automake

AM_INIT_AUTOMAKE([nostdinc dist-bzip2 check-news color-tests parallel-tests])

AC_PROG_CXX

AC_ARG_ENABLE(debug,
    [  --enable-debug          enable debug information [default=no]],
    [
        if test "$enableval" = "yes"; then
            CXXFLAGS="$CXXFLAGS -Wall -DDEBUG"
        else
            CXXFLAGS="$CXXFLAGS -Wall -DNDEBUG"
        fi
    ],[
        CXXFLAGS="$CXXFLAGS -Wall -DNDEBUG"
    ]
)

dnl Make sure we've got sensible versions of the sources (eg to get lrintf)

AC_LANG(C++)

AC_CHECK_FUNC(pthread_mutex_trylock, ,
[
    ac_save_CXXFLAGS="$CXXFLAGS"
    CXXFLAGS="$CXXFLAGS -pthread"
    AC_CHECK_FUNC(pthread_create, ,
    [
        CXXFLAGS="$ac_save_CXXFLAGS"
        AC_CHECK_LIB(pthread, pthread_create, ,
        [
            AC_MSG_WARN([Cannot find pthread library. Python lib check may fail])
        ])
    ])
])

PKG_PROG_PKG_CONFIG

PKG_CHECK_MODULES(ATLAS, atlascpp-0.6,
    [
        CPPFLAGS="$CPPFLAGS $ATLAS_CFLAGS"
        LIBS="$LIBS $ATLAS_LIBS"
    ],
    [
        AC_MSG_RESULT([no])
        AC_MSG_ERROR([
Cannot find valid versions of required WorldForge libraries:
$ATLAS_PKG_ERRORS
Perhaps you should add the directory containing the missing libraries to the
PKG_CONFIG_PATH environment variable.
Please see http://worldforge.org/dev/eng/servers/cyphesis for details of the
libraries required and where to obtain them.])
    ])

AC_CONFIG_FILES([
	Makefile
	src/Makefile
])
AC_OUTPUT
