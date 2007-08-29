# Automake input file.
# $Id$

SUBDIRS = src examples

EXTRA_DIST = phpembedVersion.h setup

# Update config if configure.ac or aclocal.m4 changes
#$(srcdir)/configure: configure.ac aclocal.m4
#	(cd $(srcdir);autoconf)
#Makefile: Makefile.in config.status
#	./config.status
#config.status: configure
#	./config.status --recheck

#This macro sets the shell variable LIBTOOL_DEPS, that can be used to automatically update libtool. 
#script if it becomes out-of-date. 
# To add to configure.ac
#AC_SUBST(LIBTOOL_DEPS)

#LIBTOOL_DEPS = @LIBTOOL_DEPS@
#libtool: $(LIBTOOL_DEPS)
#	$(SHELL) ./config.status --recheck
