#!/bin/sh
# $Id$

# Script to set up environment.

rm -rf NEWS README AUTHORS ChangeLog
rm -rf ltmain.sh configure depcomp config.guess config.sub config.h.in autom4te.cache INSTALL COPYING missing install-sh aclocal.m4 
rm -rf mkinstalldirs phpembedVersion.h config.log config.status stamp-h1 config.h libtool


touch NEWS README AUTHORS ChangeLog

echo "libtoolize..."
LIBTOOLIZE=${LIBTOOLIZE:-libtoolize}
$LIBTOOLIZE --automake

echo "aclocal..."
ACLOCAL=`which aclocal-1.9 || which aclocal19 || which aclocal-1.7 || which aclocal17 || which aclocal-1.5 || which aclocal15 || which aclocal || exit 1`
$ACLOCAL || exit 1

echo "autoheader..."
AUTOHEADER=${AUTOHEADER:-autoheader}
$AUTOHEADER || exit 1

echo "automake..."
AUTOMAKE=`which automake-1.9 || which automake-1.7 || which automake`
#$AUTOMAKE --foreign --add-missing || $AUTOMAKE --gnu --add-missing || exit 1
$AUTOMAKE -a

echo "autoconf..."
AUTOCONF=${AUTOCONF:-autoconf}
$AUTOCONF || exit 1

echo "running make clean ....."
make clean 1>/dev/null 2>&1
