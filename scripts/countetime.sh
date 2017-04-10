#!/bin/bash

BEGINTIME=`date +%s`
$@
ENDTIME=`date +%s`

TOTALTIME=`expr ${ENDTIME} - ${BEGINTIME}`
echo "Scanning took ${TOTALTIME} seconds"