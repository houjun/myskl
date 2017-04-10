#|/bin/bash

BIN=./mysklthreadtest

[ -n "$1" ] && N=$1 || N=10000

cd ../test && while [ $N -gt 0 ]
do
    #~/bin/valgrind/bin/valgrind --tool=helgrind $BIN;
    $BIN; echo $N #| gdb --batch -x /dev/stdin; echo $N
    let N=N-1
done

exit 0
