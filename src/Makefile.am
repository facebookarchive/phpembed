# Automake input file.
# $Id$

INCLUDES = -I. 
INCLUDES += @PHPCONFIGINCLUDES@

# add includes to dist.
# use noinst_LTLIBRARIES to prevent -rpath inclusion during linking.
libphpembed_includes = php_arr.h  php_cxx.h  php_stl.h  php_tok_consts.h  php_tok.h
lib_LTLIBRARIES = libphpembed.la
libphpembed_la_SOURCES = php_cxx.cpp php_arr.cpp php_stl.cpp php_tok.cpp $(libphpembed_includes)
libphpembed_la_LDFLAGS = -release 4.0.0

# Information about libtool shared lib versioning scheme.
# libtool library versions are described by three integers:
# current - The most recent interface number that this library implements.
# revision - The implementation number of the current interface.
# age - In other words, the library implements all the interface numbers in the range from number current - age to current. 

# Versioning rules
# 1. Start with version information of `0:1:0' for each libtool library.
# 2. If the library source code has changed at all since the last update, then increment revision (`c:r:a' becomes `c:r+1:a').
# 3. If any interfaces have been added, removed, or changed since the last update, increment current, and set revision to 0.
# 4. If any interfaces have been added since the last public release, then increment age.
# 5. If any interfaces have been removed since the last public release, then set age to 0. 
# Example: #libphpembed_la_LDFLAGS = -version-info 0:1:0
