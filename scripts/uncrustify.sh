#! /bin/sh

## ###################################################### ##
##                                                        ##
##                      Change here                       ##
##                                                        ##
## ###################################################### ##

MAINDIR=../UNCRUSTIFY
ME=`basename $0`

UNC_INC_OUT=$MAINDIR/modified/include
UNC_TST_OUT=$MAINDIR/modified/test
UNC_SRC_OUT=$MAINDIR/modified/src


UNC_INC_BAK=$MAINDIR/backup/include
UNC_TST_BAK=$MAINDIR/backup/test
UNC_SRC_BAK=$MAINDIR/backup/src


UNC_BIN=~/bin/uncrustify/bin/uncrustify  # `which uncrustify`
UNC_CFG=~/uncrustify.cfg

SRC_FILE="                  \
   skl_del.c                \
   skl_dst.c                \
   skl_init.c               \
   skl_iter.c               \
   skl_misc.c               \
   skl_draw.c               \
   skl_lc.c                 \
   skl_ins.c                \
   skl_join.c               \
   skl_search.c             \
   skl_gen.c                \
   private/skl_rand.c       \
   private/skl_priv.c       \
   extra/dbug/dbug.c        \
   extra/dbug/dbug-ts-ext.c \
"

INC_FILE="
   myskl.h                  \
   private/skl_protos.h     \
   private/skl_debug.h      \
   private/skl_checks.h     \
   private/skl_fatal.h      \
   private/skl_conf.h       \
   private/skl_warn.h       \
   private/skl_rand.h       \
   extra/dbug/dbug.h        \
   private/skl_priv.h       \
   extra/dbug/dbug-ts-ext.h \
   private/skl_threads.h
"

TST_FILE="                  \
   bench.c                  \
   test.c                   \
   test_func.c              \
   thread_func.c            \
   thread_test.c            \
"


## ###################################################### ##
##                                                        ##
##                      Don't touch here                  ##
##                                                        ##
## ###################################################### ##

if [ -z "$1" ]; then

   # create in <UNC_INC_BAK> and in <UNC_SRC_BAK> the
   # same dirs as in 'src' and 'include'. We use them
   # for backup files. The same way for 'uncrustified'
   # files

   mkdir -p                                        \
      $UNC_INC_BAK/extra/dbug $UNC_INC_BAK/private \
      $UNC_SRC_BAK/extra/dbug $UNC_SRC_BAK/private \
      $UNC_INC_OUT/extra/dbug $UNC_INC_OUT/private \
      $UNC_SRC_OUT/extra/dbug $UNC_SRC_OUT/private \
      $UNC_TST_OUT            $UNC_TST_BAK

   cd ../src && for item in $SRC_FILE
   do
      cp $item $UNC_SRC_BAK/$item && \
      $UNC_BIN -f $item -c $UNC_CFG > $UNC_SRC_OUT/$item
   done

   cd ../include && for item in $INC_FILE
   do
      cp $item $UNC_INC_BAK/$item && \
      $UNC_BIN -f $item -c $UNC_CFG > $UNC_INC_OUT/$item
   done

   cd ../test && for item in $TST_FILE
   do
      cp $item $UNC_TST_BAK/$item && \
      $UNC_BIN -f $item -c $UNC_CFG > $UNC_TST_OUT/$item
   done

   echo ""
   echo "******************************************"
   echo "Files saved in <$MAINDIR> directory"
   echo "******************************************"
   echo ""

elif [ "$1" = "APPLY" ]; then

   cp -fri $UNC_INC_OUT ..
   cp -fri $UNC_SRC_OUT ..
   cp -fri $UNC_TST_OUT ..

else
   echo -e "<$ME>\t\t-- backup and uncrustify files"
   echo -e "<$ME APPLY>\t-- substitute original files"
fi
