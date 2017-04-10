#!/bin/sh
# Script to run "test" under valgrind and find changes in valgrind results from last time it was run
# Usage: valgrind-daily.sh
# Example of output:
#   + Conditional jump or move depends on uninitialised value(s)
#   + Syscall param futex(val) contains uninitialised byte(s)
#   ....
#   ....
# If no difference found, no output's showed

#set -x
#set -e

ROOT_DIR=/home/andrea/Informatica/Projects/MySKL/svn/trunk
PATH_FROM_ROOT_DIR=scripts/valgrind/
TESTBIN=mysklthreadtest

PATH=`which valgrind`:$PATH
export LOGS=logs  # for 'valgrind-stats.sh' script

# Move to the root
cd $ROOT_DIR

if test ! -f $PWD/configure
then
    echo "couldn't find $PWD/configure"
    exit 1
fi

# Build a fresh copy if needed
if test ! -f $PWD/config.log
then
    ./configure --prefix=$PWD/daily/install \
                --enable-internal-debugging \
                --enable-thread-safety-full \
                --with-debug
fi
make clean

# We grep error messages, so make them all English
LANG=C

mkdir -p $LOGS
DATE=`date +%F-%H.%M`

# Finally run the test
export VALGRIND_OPTS="
    -q
    --trace-children=yes
    --track-origins=yes
    --gen-suppressions=all
    --suppressions=$PWD/scripts/valgrind/glibc-2.6.supp
    --leak-check=full
    --num-callers=20"

make install
valgrind "$PWD/test/$TESTBIN" >> $LOGS/$DATE.log 2>&1 || true

# Analyze the log
rm -f vg*.txt 2>/dev/null
rm -rf $LOGS/$DATE
mkdir $LOGS/$DATE
perl $PATH_FROM_ROOT_DIR/valgrind-split.pl $LOGS/$DATE.log
mv vg*.txt $LOGS/$DATE

sh $PATH_FROM_ROOT_DIR/valgrind-stats.sh