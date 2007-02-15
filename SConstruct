#
# DIRECTORY VARIABLES
# to keep code maximally portable, keep ALL directory references here
#

src = '#src/'
examples = '#examples/'
phpd = '/usr/local/php/include/php/'
phpdirs = [phpd, phpd+'Zend/', phpd+'TSRM/', phpd+'main/', phpd+'sapi/embed/']


#
# DEFINE BUILD ENVIRONMENT
# universal setting used for all programs for compilation
#

env = Environment(CPPFLAGS='-Wall -g',
                 CPPPATH=phpdirs + [src],
                 LINKFLAGS='`php-config --includes` `php-config --ldflags` `php-config --libs`')

libtarget = "phpembed"
libsources = [src + 'php_cxx.cpp', src + 'php_arr.cpp', 
              src + 'php_stl.cpp', src + 'php_tok.cpp']
env.StaticLibrary(target = libtarget, source = libsources)


#
# BUILD EXECUTABLES
# go to all children and execute their SConscripts
#

test = env.Program(target = 'test',
                   source = [examples + 'test.cpp', 
                             'libphpembed.a',
                             '/usr/local/php/lib/libphp5.a'],
                   LIBS = '', 
                   LIBPATH = '')

example = env.Program(target = 'example',
                      source = [examples + 'example.cpp', 
                                'libphpembed.a',
                                '/usr/local/php/lib/libphp5.a'],
                      LIBS = '', 
                      LIBPATH = '')

usage = env.Program(target = 'usage',
                    source = [examples + 'usage.cpp', 
                              'libphpembed.a',
                              '/usr/local/php/lib/libphp5.a'],
                    LIBS = '', 
                    LIBPATH = '')
