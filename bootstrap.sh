#!/bin/sh
# $Id$

# Script to set up compilation environment.

LIBTOOLIZE=${LIBTOOLIZE:-`which libtoolize`}
if test -f "${LIBTOOLIZE}"; then
   echo "libtoolize..."
   $LIBTOOLIZE --automake
else
   echo " skipping libtoolize"
fi

echo "aclocal..."
ACLOCAL=${ACLOCAL:-aclocal}
$ACLOCAL || exit 1

echo "autoheader..."
AUTOHEADER=${AUTOHEADER:-autoheader}
$AUTOHEADER || exit 1

echo "automake..."
AUTOMAKE=${AUTOMAKE:-automake}
$AUTOMAKE -a

echo "autoconf..."
AUTOCONF=${AUTOCONF:-autoconf}
$AUTOCONF || exit 1

echo "running make clean ....."
make clean 1>/dev/null 2>&1
