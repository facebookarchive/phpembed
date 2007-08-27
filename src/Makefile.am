# Automake input file.
# $Id$

INCLUDES = -I. 
#INCLUDES += @PHPCONFIGINCLUDES@
INCLUDES += -I/usr/local/php/include/php
INCLUDES += -I/usr/local/php/include/php/Zend 
INCLUDES += -I/usr/local/php/include/php/TSRM
INCLUDES += -I/usr/local/php/include/php/main
INCLUDES += -I/usr/local/php/include/php/sapi/embed

# add includes to dist.
libphpembed_includes = php_arr.h  php_cxx.h  php_stl.h  php_tok_consts.h  php_tok.h
lib_LTLIBRARIES = libphpembed.la
libphpembed_la_SOURCES = php_cxx.cpp php_arr.cpp php_stl.cpp php_tok.cpp $(libphpembed_includes)

# old style wihtout libtools
#lib_LIBRARIES = libphpembed.a
#libphpembed_a_SOURCES = php_cxx.cpp php_arr.cpp php_stl.cpp php_tok.cpp $(libphpembed_includes)
