
## Process this file with automake to produce Makefile.in

INCLUDES= -I$(top_srcdir)/include/private  \
          -I$(top_srcdir)/include

libmyskl_LDADD = $(top_builddir)/src/libmyskl.la

################################################
#                                              #
# NOTICE::                                     #
# the names between '[' and ']' must be equal  #
# as well as the names between '{' and '}'     #
#                                              #
# [test]dir = ...                              #
# [test]_PROGRAMS = {myskltest}                #
# {myskltest}_SOURCES = ...                    #
# {myskltest}_LDADD   = ...                    #
#                                              #
################################################

# NOTE for me: @VAR@ is the same as $(VAR)

############
## test 1 ##
############

testdir = $(PWD)
test_PROGRAMS = myskltest

myskltest_SOURCES = test_func.c test.c

# Automake allows to set compilation flags on a per-program
# (or per-library) basis. The use of per-target compilation
# flags with C sources requires that the macro AM_PROG_CC_C_O
# be called from configure.ac. In turn, this macro requires
# the script ./compile. 'automake --add-missing' creates it

myskltest_CFLAGS = @MYSKL_DLL_IMPORT@
myskltest_LDADD  = $(libmyskl_LDADD)

############
## test 2 ##
############

if HAVE_THREADS
if THREAD_FULL_MODE

threadtestdir = $(PWD)
threadtest_PROGRAMS = mysklthreadtest

mysklthreadtest_SOURCES = test_func.c thread_func.c thread_test.c
mysklthreadtest_CFLAGS  = @MYSKL_DLL_IMPORT@
mysklthreadtest_LDADD   = $(libmyskl_LDADD)

endif
endif

############
## bench  ##
############

benchdir = $(PWD)
bench_PROGRAMS = mysklbench

mysklbench_SOURCES = bench.c
mysklbench_CFLAGS  = @MYSKL_DLL_IMPORT@
mysklbench_LDADD   = $(libmyskl_LDADD)


# also include these files when packaging
EXTRA_DIST = *.h