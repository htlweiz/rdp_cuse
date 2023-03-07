#!/bin/bash

CMD=$1
shift
IN_FILE=$1
shift
OUT_FILE=$1
shift

${CMD} < ${IN_FILE} > ${OUT_FILE}.IS

diff ${OUT_FILE} ${OUT_FILE}.IS > /dev/null
result=$?

diff -u --color=always ${OUT_FILE} ${OUT_FILE}.IS | tail +4 | sed s/^/\#\#\#/ | sed s/$/\#\#\#/

rm -f ${OUT_FILE}.IS

exit ${result}

