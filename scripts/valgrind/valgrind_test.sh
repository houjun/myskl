#!/bin/bash

ME=`basename $0`

DEFAULT_TOOLS='memcheck+helgrind'

# <maxLev1>,<maxLev2>,<elemNu1>,<elemNu2>,<max_keyvalue>
BINARGS=""
#BINARGS="50 60 1000 1000 1000"

BINPATH=../../test/myskltest
LDSPATH=glibc-2.6.supp

MSCACHE=massif_cache     # massif
CGCACHE=callgrind_cache  # callgrind
MCCACHE=memcheck_cache   # memcheck
HGCACHE=helgrind_cache   # helgrind

# if empty valgrind will be searched by using 'which'
VALGRIND=~/bin/valgrind/bin/valgrind

EDITOR=kwrite
KCGBIN=kcachegrind


function clear_old_istances
{
    hookstr+="($KCGBIN.+callgrind.+)|"
    hookstr+="($EDITOR.+memcheck.+)|"
    hookstr+="($EDITOR.+helgrind.+)|"
    hookstr+="($EDITOR.+massif.+)"

    ps aux | grep -iE $hookstr  |\
    grep -v  grep               |\
    awk '{print $2}'            |\
    xargs kill -9 2>/dev/null
}


function usage
{
    echo "Usage: $ME [massif[+memcheck][+callgrind][+helgrind]] [close|help|clear|binpath]"
    echo "       $ME -- only with internal DEFAULT_TOOLS variable set"
    echo "       $ME close -- close all opened istances (kwrite)"
    echo "       $ME clear -- delete valgrind output files"
    echo "       $ME help"
}


if [ -z "$VALGRIND" ]; then
    VALGRIND=`which valgrind`
    [ $? -eq 1 ] && echo "Error: valgrind not found" && exit 1
fi

if [ $# -gt 0 ]; then

    # get last arg: ${!$#} doesn't work
    arg=$# && last_arg=${!arg}

    if [ "$last_arg" == "clear" ]; then
        rm -f massif.*    \
              helgrind.*  \
              callgrind.* \
              memcheck.*  2>/dev/null
        exit 0

    elif [ "$last_arg" == "close" ]; then
        clear_old_istances
        exit 0

    elif [ "$last_arg" == "help"  ]; then
        usage
        exit 0

    elif [[ $last_arg == binpath* ]]; then

        # binpath from command line
        BINPATH=${last_arg/*:/}

        if [ $# -eq 1 ]; then
            tools=$DEFAULT_TOOLS
        else
            tools=$1
        fi
    else
        tools=$1
    fi
else
    if [ -n "$DEFAULT_TOOLS" ]; then
       tools=$DEFAULT_TOOLS
    else
       echo "Error: var DEFAULT_TOOLS not set"
       exit 1
    fi
fi

tools=${tools//+/ }
clear_old_istances

for tool in $tools; do

    if [ "$tool" == "memcheck"  ]; then

        mkdir -p $MCCACHE
        mv memcheck* $MCCACHE 2>/dev/null

        mc_tmpfile="memcheck.out.`date +%s`"
        $VALGRIND                       \
            --leak-check=full           \
            --show-reachable=yes        \
            --suppressions=$LDSPATH     \
            $BINPATH $BINARGS 2> $mc_tmpfile #2>&1

       #$VALGRIND                       \
       #     --leak-check=full           \
       #     --show-reachable=yes        \
       #     --suppressions=$LDSPATH     \
       #     $BINPATH $@ 2> $mc_tmpfile #2>&1

        $EDITOR $mc_tmpfile &
    fi

    if [ "$tool" == "massif" ]; then

        mkdir -p $MSCACHE
        mv massif* $MSCACHE 2>/dev/null

        # generates: massif.out.<pid>
        $VALGRIND --tool=massif $BINPATH $BINARGS

        ms_tmpfile="massif.out.`date +%s`"
        ms_print massif.* > $ms_tmpfile

        $EDITOR $ms_tmpfile &
    fi

    if [ "$tool" == "callgrind"  ]; then

        mkdir -p $CGCACHE
        mv callgrind* $CGCACHE 2>/dev/null

        # generates: callgrind.out.<pid>
        $VALGRIND --tool=callgrind $BINPATH $BINARGS

        $KCGBIN callgrind.* &
    fi

    if [ "$tool" == "helgrind" ]; then

        mkdir -p $HGCACHE
        mv helgrind* $HGCACHE 2>/dev/null

        hg_tmpfile="helgrind.out.`date +%s`"
        #$VALGRIND --tool=helgrind --read-var-info=yes $BINPATH $BINARGS > $hg_tmpfile 2>&1
        $VALGRIND --tool=helgrind $BINPATH $BINARGS > $hg_tmpfile 2>&1

        $EDITOR $hg_tmpfile &
    fi
done



# valout=`ls -rtl | tail -2`
 #$VIEWER --orientation seascape `echo $valout | cut -d ' ' -f2` &
       # find last file created
        #valout=`ls -rt -I $MSCACHE | tail -1`