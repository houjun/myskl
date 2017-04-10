#! /bin/sh

WD=`pwd`          # <scripts> dir
ROOTDIR=${WD%/*}  # <rootdir/scripts> ---> <rootdir>


###################################################################################
###################################################################################

BUILDROOT="build"
CONFIGURE="$ROOTDIR/configure --prefix=$ROOTDIR/$BUILDROOT/install"
MAKE="make install"
MKDIR="mkdir -p"
RM="rm -f"
CP="cp -f"

# relative paths from <scripts> directory

VALGRIND_DIR=/valgrind
VALGRIND_BIN=$VALGRIND_DIR/valgrind_test.sh
VALGRIND_SUP=$VALGRIND_DIR/glibc-2.6.supp

TESTSETS_BIN=test_launcher.sh

# relative paths from <$BUILDROOT> directory

MYSKL_BINPATH_1=test/mysklthreadtest
MYSKL_BINPATH_2=test/mysklbench
MYSKL_BINPATH_3=test/myskltest

DISABLE=0

###################################################################################
###################################################################################

function runtests
{
    [ $1 -eq 1 ] && return

    # avoid that 'make install' complains about already existing files

    $RM $BUILDDIR/$MYSKL_BINPATH_1 \
        $BUILDDIR/$MYSKL_BINPATH_2 \
        $BUILDDIR/$MYSKL_BINPATH_3

    $MKDIR $BUILDDIR                        &&  \
           $CP $WD/$VALGRIND_SUP $BUILDDIR  &&  \
           cd $BUILDDIR                     &&  \
           [ ! -e config.h ] && ( $CONFIGURE $CONFOPTS && $MAKE ) || $MAKE

    # make error
    [ $? -ne 0 ] && return

    [ -n "$TEST_TESTSETS" ] && $TEST_TESTSETS
    [ -n "$TEST_VALGRIND" ] && $TEST_VALGRIND

    [ -n "$TEST_THR_TESTSETS" ] && $TEST_THR_TESTSETS
    [ -n "$TEST_THR_VALGRIND" ] && $TEST_THR_VALGRIND

    resetvar
}

function resetvar
{
    TEST_THR_VALGRIND=
    TEST_THR_TESTSETS=

    TEST_TESTSETS=
    TEST_VALGRIND=

    BUILDDIR=
    CONFOPTS=
}

function presskey
{
    [ $1 -eq 1 ] && return

    # gives valgrind the time to outuput its final stuff
    sleep 3

    echo -en "\nPress any key to continue..."
    read answer
}


###################################################################################
###################################################################################

#### config 1 ####

BUILDDIR=$ROOTDIR/$BUILDROOT/enable-internal-debugging
CONFOPTS="--enable-internal-debugging"

TEST_VALGRIND="$WD/$VALGRIND_BIN binpath:$BUILDDIR/test/myskltest"
TEST_TESTSETS="$WD/$TESTSETS_BIN 100 random $BUILDDIR/test/myskltest"

runtests $DISABLE
presskey $DISABLE


#### config 2 ####

resetvar

BUILDDIR=$ROOTDIR/$BUILDROOT/default-options

TEST_VALGRIND="$ROOTDIR/scripts/$VALGRIND_BIN binpath:$BUILDDIR/test/myskltest"
TEST_TESTSETS="$WD/$TESTSETS_BIN 100 random $BUILDDIR/test/myskltest"

runtests $DISABLE
presskey $DISABLE


#### config 3 ####

resetvar

BUILDDIR=$ROOTDIR/$BUILDROOT/enable-thread-safety
CONFOPTS="--enable-thread-safety"

TEST_VALGRIND="$WD/$VALGRIND_BIN binpath:$BUILDDIR/test/myskltest"
TEST_TESTSETS="$WD/$TESTSETS_BIN 100 random $BUILDDIR/test/myskltest"

runtests $DISABLE
presskey $DISABLE


#### config 4 ####

resetvar

BUILDDIR=$ROOTDIR/$BUILDROOT/enable-thread-safety_enable-internal-debugging
CONFOPTS="--enable-thread-safety --enable-internal-debugging"

TEST_VALGRIND="$WD/$VALGRIND_BIN binpath:$BUILDDIR/test/myskltest"
TEST_TESTSETS="$WD/$TESTSETS_BIN 100 random $BUILDDIR/test/myskltest"

runtests $DISABLE
presskey $DISABLE


#### config 5 ####

resetvar

BUILDDIR=$ROOTDIR/$BUILDROOT/enable-thread-safety-full_enable-internal-debugging
CONFOPTS="--enable-thread-safety-full --enable-internal-debugging"

TEST_VALGRIND="$WD/$VALGRIND_BIN binpath:$BUILDDIR/test/myskltest"
TEST_TESTSETS="$WD/$TESTSETS_BIN 100 random $BUILDDIR/test/myskltest"
TEST_THR_VALGRIND="$WD/$VALGRIND_BIN binpath:$BUILDDIR/test/mysklthreadtest"

runtests $DISABLE
presskey $DISABLE


#### config 6 ####

resetvar

BUILDDIR=$ROOTDIR/$BUILDROOT/libgc_enable-thread-safety_enable-internal-debugging
CONFOPTS="--with-libgc=yes --enable-thread-safety --enable-internal-debugging"

TEST_TESTSETS="$WD/$TESTSETS_BIN 100 random $BUILDDIR/test/myskltest"

runtests $DISABLE
presskey $DISABLE


#### config 7 ####

resetvar

BUILDDIR=$ROOTDIR/$BUILDROOT/libgc_enable-thread-safety-full_enable-internal-debugging
CONFOPTS="--with-libgc=yes --enable-thread-safety-full --enable-internal-debugging"

TEST_TESTSETS="$WD/$TESTSETS_BIN 100 random $BUILDDIR/test/myskltest"

runtests $DISABLE
presskey $DISABLE


#### config 8 ####

resetvar

BUILDDIR=$ROOTDIR/$BUILDROOT/libgc_enable-thread-safety
CONFOPTS="--with-libgc=yes --enable-thread-safety"

TEST_TESTSETS="$WD/$TESTSETS_BIN 100 random $BUILDDIR/test/myskltest"

runtests $DISABLE
presskey $DISABLE


#### config 9 ####

resetvar

BUILDDIR=$ROOTDIR/$BUILDROOT/libgc_enable-thread-safety-full
CONFOPTS="--with-libgc=yes --enable-thread-safety-full"

TEST_TESTSETS="$WD/$TESTSETS_BIN 100 random $BUILDDIR/test/myskltest"

runtests $DISABLE
presskey $DISABLE


#### config 10 ####

resetvar

BUILDDIR=$ROOTDIR/$BUILDROOT/libgc_enable-internal-debugging
CONFOPTS="--with-libgc=yes --enable-internal-debugging"

TEST_TESTSETS="$WD/$TESTSETS_BIN 100 random $BUILDDIR/test/myskltest"

runtests $DISABLE
presskey $DISABLE


#### config 11 ####

resetvar

BUILDDIR=$ROOTDIR/$BUILDROOT/libgc
CONFOPTS="--with-libgc=yes"

TEST_TESTSETS="$WD/$TESTSETS_BIN 100 random $BUILDDIR/test/myskltest"

runtests $DISABLE
presskey $DISABLE

exit 0
