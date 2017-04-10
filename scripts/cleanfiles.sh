#!/bin/bash

cd ..
for entry in `find . -regex '.+\.[h|c]'`
do
    mv $entry $entry.bak

    # 1)
    #   Strip out feed chars, fixing the tedious message:
    #   <...... warning: invalid white space character in directive>
    #
    #   This problem is due to the fact that the DOS-based and UNIX
    #   based text editors treat a line of text differently. In the
    #   DOS-based text editor, the end of a line is marked by two
    #   characters ('\n' and '\r') while the UNIX based text editor
    #   use only '\n'. This script permits to get rid of the extra
    #   white space using tr (translate characters)
    #
    # 2)
    #   remove all leading and trailing whitespace from end of each
    #   line using sed

    cat $entry.bak | tr -d '\015' | sed 's/[ \t]\+$//' > $entry
done
