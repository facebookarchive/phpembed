# Autoconf input file
# $Id$

AC_PREREQ(2.52)
AC_INIT(phpembed, 4.0.0, boz@facebook.com)

AC_CANONICAL_SYSTEM
AC_CONFIG_SRCDIR(phpembedVersion.h.in)
AM_INIT_AUTOMAKE(phpembed, 2.0.0)
AM_CONFIG_HEADER(config.h)

AC_PREFIX_DEFAULT(`pwd`/install)

AC_PROG_CPP
AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB(RANLIB, ranlib)

############################################################
# AC_DISABLE_SHARED Turns off shared lib creation. Use  --enable-shared=pkg1,pkg2
# This implies all libs are built static except pkg1, pkg2
# --enable-static=pkg3,pkg4 . Implies shared is enabled except for pkg3 and pkg4

# When developing a package use --disable-shared OR AC_DISABLE_SHARED macro.
# Current Default shared
#AC_DISABLE_SHARED
AC_PROG_LIBTOOL

##############################################################

AC_PROG_INSTALL
cache_file="config.cache"

# sets the variables BASH, PERL, RANLIB 
AC_PATH_PROGS(BASH, bash)
AC_PATH_PROGS(PERL, perl)
AC_PATH_PROGS(PYTHON, python)
AC_PATH_PROGS(AR, ar)
AC_PATH_PROGS(ANT, ant)

# Checks for header files.
#AC_INCLUDES_DEFAULT
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h fcntl.h limits.h netdb.h netinet/in.h stdint.h stdlib.h string.h sys/param.h sys/socket.h sys/time.h unistd.h])
AC_HEADER_STDBOOL
AC_HEADER_TIME

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_C_VOLATILE

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_STAT
AC_FUNC_STRTOD
AC_CHECK_FUNCS([gethostname gettimeofday memset socket strchr strerror strstr strtoul strtoull])

# Path to php-config program
AC_ARG_WITH(php-config,
    [ --with-php-config=PATH  Path to php-config tool. EX: /<localpath>/php-config],
    [PHPCONFIG="$withval"],
    [AC_PATH_PROGS(PHPCONFIG, php-config, , $PATH)]
)
if test -z "${PHPCONFIG}"; then
   AC_MSG_ERROR([Program php-config not found. Required for further procecssing. Use --with-php-config=path to specify absolute path to the php-config tool.])
fi

AC_SUBST(PHPCONFIG)

# List of php-config include paths
AC_ARG_WITH(php-includes,
    [ --with-php-includes=PATHS  php-config include locations.],
    [PHPCONFIGINCLUDES="$withval"],
    [PHPCONFIGINCLUDES=""]
)
if test -z "${PHPCONFIGINCLUDES}"; then
   PHPCONFIGINCLUDES=`$PHPCONFIG --includes`
   PHPCONFIGINCLUDES="${PHPCONFIGINCLUDES} -I`$PHPCONFIG --prefix`/include/php/sapi/embed"
fi
AC_SUBST(PHPCONFIGINCLUDES)

# checking some basic php-config  headers
save_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS ${PHPCONFIGINCLUDES}"
headerstocheck="php_config.h php_version.h zend.h zend_API.h php.h php_ini.h ext/standard/info.h"
for file in ${headerstocheck}
do
    AC_PREPROC_IFELSE([$file], [AC_MSG_RESULT([${file} - ok])], AC_MSG_ERROR([Cannot find $file.]))
done

# Special check for php_embed.h
embedheader="php_embed.hh"
AC_PREPROC_IFELSE([$embedheader], [AC_MSG_RESULT([${embedheader} - ok])], AC_MSG_ERROR([Cannot find ${embedheader}. Check if php was compiled with the --enable-embed option.]))

NEWCPPFLAGS="$CPPFLAGS"
CPPFLAGS=$save_CPPFLAGS

# List of php-config ldflags
AC_ARG_WITH(php-ldflags,
    [ --with-php-ldflags=LDFLAGS  php-config ldflags list.],
    [PHPCONFIGLDFLAGS="$withval"],
    [PHPCONFIGLDFLAGS=""]
)
if test -z "${PHPCONFIGLDFLAGS}"; then
   PHPCONFIGLDFLAGS=`$PHPCONFIG --ldflags`
   PHPCONFIGLDFLAGS="${PHPCONFIGLDFLAGS} -L`$PHPCONFIG --prefix`/lib"
fi
AC_SUBST(PHPCONFIGLDFLAGS)

# List of php-config include paths
AC_ARG_WITH(php-libs,
    [ --with-php-libs=LIBS  php-config lib locations.],
    [PHPCONFIGLIBS="$withval"],
    [PHPCONFIGLIBS=""]
)
if test -z "${PHPCONFIGLIBS}"; then
   PHPCONFIGLIBS=`$PHPCONFIG --libs`
   PHPCONFIGLIBS="${PHPCONFIGLIBS} -L`$PHPCONFIG --prefix`/lib"
fi
AC_SUBST(PHPCONFIGLIBS)

AC_MSG_CHECKING(whether to use ccache)
AC_ARG_WITH(ccache,
  [  --with-ccache        Use ccache.],
  [ case "${with_ccache}" in
      no)
      ;;
      "" | yes)
        CC="ccache ${CC}"
        CXX="ccache ${CXX}"
        with_ccache="yes"
        #AC_ERROR(The compilers are ${CC} ${CXX}.)
        ;;
      *)
        AC_ERROR(Option --with-ccache will only accept yes or no arguments. Arg given:${with_ccache}.)
        ;;
    esac ],[
    with_ccache="no"
    ]
)
AC_MSG_RESULT($with_ccache)

AC_MSG_CHECKING([whether to enable release build])
AC_ARG_ENABLE(release,
  [  --enable-release     Set CFLAGS DEFS for release build.],
  [
    # If the user wants something different than last time, die.
    # Debug and release/final builds should be done in different trees.
    if test ! -z "$php_cv_enable_release" \
        -a "$enable_release" != "$php_cv_enable_release"; then
        AC_ERROR( Debug level changed!!
        Configure should be run in a separate directory.
        (If you insist on configuring here, remove $cache_file
        and 'make clean' before compiling.)
        )
    fi
    case "${enableval}" in
    no)
        php_cv_enable_release=no
        ;;
    "" | yes)
        if test x$enable_debug = "xyes"; then
            AC_MSG_RESULT(NO!)
            AC_MSG_ERROR([Can only specify one of: --enable-debug,release.])
        else
            php_cv_enable_release=yes
            AC_DEFINE([RELEASE], [1], [Define])
        fi
        CFLAGS="-O3"
        CXXFLAGS="-O3"
        ;;
    esac ],[
    # Default to 'no' unless it was something else last time.
    if test ! -z "$php_cv_enable_release" \
        -a "$php_cv_enable_release" != "no"; then
        AC_ERROR( Debug level changed!!
        Configure should be run in a separate directory.
        (If you insist on configuring here, remove $cache_file
        and 'make clean' before compiling.)
        )
    fi
    enable_release=no
    php_cv_enable_release=no
    ]
)
AC_MSG_RESULT($enable_release)


AC_MSG_CHECKING([whether to enable debug build])
AC_ARG_ENABLE(debug,
  [  --enable-debug     Set CFLAGS DEFS for release build.],
  [
    # If the user wants something different than last time, die.
    # Debug and release/final builds should be done in different trees.
    if test ! -z "$php_cv_enable_debug" \
        -a "$enable_debug" != "$php_cv_enable_debug"; then
        AC_ERROR( Debug level changed!!
        Configure should be run in a separate directory.
        (If you insist on configuring here, remove $cache_file
        and 'make clean' before compiling.)
        )
    fi
    case "${enableval}" in
    no )
        php_cv_enable_debug=no
        ;;
    * )
        if test x$enable_release = "xyes"; then
            AC_MSG_RESULT(NO!)
            AC_MSG_ERROR([Can only specify one of: --enable-debug,--enable-release.])
        else
            php_cv_enable_debug=yes
        fi
        CFLAGS="$CFLAGS -O3"
        CXXFLAGS="$CXXFLAGS -O3"
        ;;
    esac ],[
    if test x$enable_release = "xyes"; then
        enable_debug=no
    else
        enable_debug=yes
        CFLAGS="-g"
        CXXFLAGS="-g"
    fi
    if test ! -z "$php_cv_enable_debug" \
        -a "$php_cv_enable_debug" != "$enable_debug"; then
        AC_ERROR( Debug level changed!!
        Configure should be run in a separate directory.
        (If you insist on configuring here, remove $cache_file
        and 'make clean' before compiling.)
        )
    fi
    php_cv_enable_debug=$enable_debug
    ]
)
AC_MSG_RESULT($enable_debug)


# phpembed versioning
PHPEMBED_MAJOR_VERSION=`echo $VERSION | cut -f1 -d.`
PHPEMBED_MINOR_VERSION=`echo $VERSION | cut -f2 -d.`
PHPEMBED_PATCH_VERSION=`echo $VERSION | cut -f3 -d.`
PHPEMBED_VERSION_NUMBER="$PHPEMBED_MAJOR_VERSION $PHPEMBED_MINOR_VERSION $PHPEMBED_PATCH_VERSION"
PHPEMBED_VERSION_STRING=\"$VERSION\"

AC_SUBST(PHPEMBED_MAJOR_VERSION)
AC_SUBST(PHPEMBED_MINOR_VERSION)
AC_SUBST(PHPEMBED_PATCH_VERSION)
AC_SUBST(PHPEMBD_VERSION_NUMBER)
AC_SUBST(PHPEMBED_VERSION_STRING)

# Add common compiler options.
CFLAGS="$CFLAGS -Wall"
CXXFLAGS="$CXXFLAGS -Wall"

AC_CONFIG_FILES(Makefile phpembedVersion.h src/Makefile examples/Makefile)
AC_OUTPUT

echo "PHPCONFIG -- $PHPCONFIG"
echo "PHPCONFIGINCLUDES -- $PHPCONFIGINCLUDES"
echo "PHPCONFIGLDFLAGS -- $PHPCONFIGLDFLAGS"
echo "PHPCONFIGLIBS -- $PHPCONFIGLIBS"
echo "CFLAGS = $CFLAGS"
echo "CXXFLAGS = $CXXFLAGS"
echo "CPPFLAGS = $NEWCPPFLAGS"
echo "DEFS = $DEFS"
echo "CC  = $CC"
echo "CXX = $CXX"
echo "VERSION = $VERSION"
echo "PHPEMBED_VERSION_NUMBER= ${PHPEMBED_MAJOR_VERSION}"
echo "PHPEMBED_VERSION_NUMBER= ${PHPEMBED_VERSION_NUMBER}"
