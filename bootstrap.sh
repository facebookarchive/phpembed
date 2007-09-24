#!/bin/sh
# $Id$

# Script to set up compilation environment.

touch AUTHORS ChangeLog

LIBTOOLIZE=${LIBTOOLIZE:-`which libtoolize`}
if test -f "${LIBTOOLIZE}"; then
   echo "libtoolize..."
   $LIBTOOLIZE --automake -c
else
   echo "Error libtoolize missing. Must install libtools package."
   exit 1
fi

echo "aclocal..."
ACLOCAL=${ACLOCAL:-aclocal}
$ACLOCAL || exit 1

echo "autoheader..."
AUTOHEADER=${AUTOHEADER:-autoheader}
$AUTOHEADER || exit 1

echo "automake..."
AUTOMAKE=${AUTOMAKE:-automake}
$AUTOMAKE -ac

echo "autoconf..."
AUTOCONF=${AUTOCONF:-autoconf}
$AUTOCONF || exit 1

make clean 1>/dev/null 2>&1
