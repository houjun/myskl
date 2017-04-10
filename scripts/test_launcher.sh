#! /bin/sh

#########################################################################
#
# Call test program repeatedly: first setting max keys value to 1, then
# to 2,... and finally to <max_keyvalue_threshold>
#
# Usage:    <scriptname.sh [max_keyvalue_threshold] [random] [binprog]>
# Examples: <scriptname.sh 50 random [binprog]>
#           <scriptname.sh 50> (default values for max levels, elements
#                               number and binary program)
#           <scriptname.sh>    (default values for all)
#
########################################################################

# default values
MAX_DEFAULT_KEYVALUE_THRESHOLD=5
MAX_LEVEL_1=5
MAX_LEVEL_2=3
MAX_NELEM_1=7
MAX_NELEM_2=3

# other stuff
ERRPATTERN="test failed\|leak"
RESFILE=test_max_key_value
DIRPATH=../test
BINNAME=myskltest
TESTDIR=testsets
EXT=.txt

########################################################################

function check_if_interactive_mode_off
{
    program=$1
    echo -n "Checking if program is compiled with interactive mode off..."

    ($program 1 1 1 1 1 1> /dev/null 2> /dev/null & ) && sleep 2
    pid=`ps aux | grep -i "libs/$BINNAME\| $BINNAME " | grep -v grep | awk '{print $2}'`

    if [ -n "$pid" ]; then
        echo "[NO]. Recompile it!!"
        kill -9 $pid 2>/dev/null

        # can't go on
        exit
    else
        echo "[OK]"
    fi
}

########################################################################

if [ $3 ]; then
    # custom value for binpath and binname
    BINNAME=`basename $3`; binpath=$3
else
    # default value for binpath and binname
    binpath=$DIRPATH/$BINNAME
fi

max_keyvalue_threshold=$MAX_DEFAULT_KEYVALUE_THRESHOLD
cur_max_keyvalue=1

[ $1 ] && max_keyvalue_threshold=$1; mkdir -p $TESTDIR
[ "$2" = "random" ] && let MAX_LEVEL_1=$RANDOM%20+1  && \
                       let MAX_LEVEL_2=$RANDOM%20+1  && \
                       let MAX_NELEM_1=$RANDOM%100+1 && \
                       let MAX_NELEM_2=$RANDOM%100+1

check_if_interactive_mode_off $binpath

echo "Testing using these parameters..."
echo "Max level 1: $MAX_LEVEL_1"
echo "Max level 2: $MAX_LEVEL_2"
echo "Max #elem 1: $MAX_NELEM_1"
echo "Max #elem 2: $MAX_NELEM_2"

echo -e "\nBEGINNING TESTING...\n"
while [ "$cur_max_keyvalue" -le $max_keyvalue_threshold ]
do
    # <maxLev1>,<maxLev2>,<elemNu1>,<elemNu2>,<max_keyvalue>
    $binpath                                            \
        $MAX_LEVEL_1                                    \
        $MAX_LEVEL_2                                    \
        $MAX_NELEM_1                                    \
        $MAX_NELEM_2                                    \
        $cur_max_keyvalue                               \
            1> $TESTDIR/$RESFILE[$cur_max_keyvalue]$EXT \
            2> $TESTDIR/$RESFILE[$cur_max_keyvalue]$EXT

    res=`grep -Hi "$ERRPATTERN" $TESTDIR/$RESFILE[$cur_max_keyvalue]$EXT`
    if [ -n "$res" ]; then echo "$res"; echo; fi

    let cur_max_keyvalue=$cur_max_keyvalue+1
done
echo "END"