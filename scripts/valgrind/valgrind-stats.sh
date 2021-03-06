#!/bin/sh
# Usage: sh tools/valgrind/valgrind-stats.sh
# Compute statistics for the most recent run
#set -x
#set -e

if [ -z "$LOGS" ]; then echo "Error!! Var $LOGS not defined"; exit; fi
cd $LOGS

DATE=`ls -d ????-??-??-??.?? | tail -1`
PREV=`ls -d ????-??-??-??.?? | tail -2 | head -1`

PATTERN="are definitely|uninitialised|Unhandled exception|Invalid read|Invalid write|Invalid free|Source and desti|Mismatched free|unaddressable byte|vex x86|impossible|Assertion|INTERNAL ERROR|Terminated|Killed"

# Generate histogram of errors
cat $DATE.log | egrep -C3 "$PATTERN" | grep == | sed 's/.*=//' > sum1
cat sum1 |
    sed "/Warning: set add/s/.*//" |
    sed "/ERROR SUMMARY/s/.*//" |
    sed "/malloc.free/s/.*//" |
    sed "/Reachable blocks/s/.*//" |
    sed "/searching for pointer/s/.*//" |
    sed "/Your program just tried to execute an instruction that Valgrind/s/.*//" |
    sed "/did not recognise. There are two possible reasons for this.*/s/.*//" |
    sed "/More than 100 errors detected/s/.*//" |
    sed "/will still be recorded/s/.*//" |
    sed "/For counts of detected/s/.*//" |
    sed "/suppressed:/s/.*//" |
    sed "/Thread [0-9]*:/s/.*//" |
    sed "/^yes/s/.*//" |
    cat > sum2
cat sum2 | sed 's/by 0x[0-9a-fA-F]*:/by /' | sed 's/at 0x[0-9a-fA-F]*:/at /' > sum3
sed 's/^ \([^ ]\)/|\1/' < sum3 > sum4
cat sum4 | tr '\012' '\011' | tr '|' '\012' | sed 's/  */ /g' | sed 's/[ 	]*$//' | grep . > sum5
sort < sum5 | uniq -c | sort -rn > $DATE-count-by-error.txt

# Generate count of errors by file
for a in `cd $DATE; ls vg*.txt | grep -v .-diff`
do
    echo -n "$a    "
    egrep  "$PATTERN" < $DATE/$a | wc -l || true
done | sort --k 2n > $DATE-count-by-file.txt

# Generate short list of changes since previous day
diff -Nu  -x '*diff.txt' $PREV $DATE >  $DATE-diff.txt || true
cat $DATE-diff.txt | egrep '^[-+].*('"$PATTERN"')|diff' >  $DATE-summary.txt || true

#set +e

# Generate brutally minimalist diffs for each test.
cd $DATE
for file in `ls vg*.txt | grep -v .-diff.txt`
do
    out=`echo $file | sed 's/vg-/diff-/'`
    # Prepare a cleaned-up version of each file, removing
    # parts that change rapidly and don't help identify valgrind
    # warnings.
    for foo in $file ../$PREV/$file
    do
      sed "\
        s/^0x[0-9a-f]*:.*/0xNNNN/g;\
        s/0x[0-9a-f]*/0xNNNN/g;\
        s/^ELF.*/ELF/;\
        s/\\-PE.*//;\
        s/\.c:[0-9]*/.c/;\
        s/[,0-9]* bytes in /NNN bytes in /;\
        s/(thread [0-9a-f]*)/(thread XX)/\
        " "$foo" > $foo.x #$foo | perl ../../tools/valgrind/skip-backtrace.pl > $foo.x || touch $foo.x
    done
    # Ignore empty diffs or diffs that do not contain a valgrind warning
    if diff -Nu ../$PREV/$file.x $file.x > $out || ! egrep '^[-+].*('"$PATTERN"')' $out
    then
       rm $out
    fi
    rm ../$PREV/$file.x $file.x
done
