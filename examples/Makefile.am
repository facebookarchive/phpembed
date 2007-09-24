# Automake input file.
# $Id$

# Includes paths
INCLUDES = -I. 
INCLUDES += @PHPCONFIGINCLUDES@
INCLUDES += -I$(top_srcdir)/src

# Paths to installed libraries
AM_LDFLAGS = @PHPCONFIGLDFLAGS@

# List of libraries to link with.
LDADD = @PHPCONFIGLIBS@ $(top_builddir)/src/libphpembed.la -lphp5

bin_PROGRAMS = test usage
test_SOURCES = test.cpp
usage_SOURCES = usage.cpp

