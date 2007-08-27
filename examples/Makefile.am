# Automake input file.
# $Id$

# Includes paths
INCLUDES = -I. 
INCLUDES += @PHPCONFIGINCLUDES@
INCLUDES += -I$(top_srcdir)/src
INCLUDES += -I/usr/local/php/include/php
INCLUDES += -I/usr/local/php/include/php/Zend 
INCLUDES += -I/usr/local/php/include/php/TSRM
INCLUDES += -I/usr/local/php/include/php/main
INCLUDES += -I/usr/local/php/include/php/sapi/embed

# Paths to installed libraries
AM_LDFLAGS = @PHPCONFIGLDFLAGS@ -L/usr/local/php/lib

# List of libraries to link with.
LDADD = @PHPCONFIGLIBS@ $(top_builddir)/src/libphpembed.a -lphp5


bin_PROGRAMS = test usage
test_SOURCES = test.cpp
usage_SOURCES = usage.cpp
