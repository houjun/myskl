
#################################################
#                                               #
# We use libtool which provides a reliable and  #
# generalized library-building support services #
#                                               #
#################################################

# Required includes

INCLUDES= -I$(top_srcdir)/include/extra/dbug \
          -I$(top_srcdir)/include/private

# Library object files archive

lib_LTLIBRARIES = libmyskl.la

if INTERNAL_DEBUGGING_ENABLED
if HAVE_THREADS
internal_dbug=              \
    extra/dbug/dbug.c       \
    extra/dbug/dbug-ts-ext.c
else
internal_dbug= extra/dbug/dbug.c
endif
endif

if THREAD_FULL_MODE
rwlock=private/rwlock.c
endif

libmyskl_la_SOURCES =       \
                            \
    private/skl_priv.c      \
    private/skl_rand.c      \
    $(rwlock)		    \
    $(internal_dbug)        \
    skl_del.c               \
    skl_gen.c               \
    skl_draw.c              \
    skl_dst.c               \
    skl_lc.c                \
    skl_init.c              \
    skl_ins.c               \
    skl_iter.c              \
    skl_join.c              \
    skl_misc.c              \
    skl_search.c

# For the runtime loader. Set up in
# <configure.in>

libmyskl_la_LIBADD = $(THREADLIBS) $(GCLIBS)

libmyskl_la_LDFLAGS = \
-no-undefined -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)
