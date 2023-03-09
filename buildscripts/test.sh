#!/bin/bash

CMD=$1
shift
IN_FILE=$1
shift
OUT_FILE=$1
shift

[ -e "${CMD}" ] || {
  echo "$(basename ${CMD}) not yet implemented" >&2
  exit 1
} 

${CMD} < ${IN_FILE} > ${OUT_FILE}.IS

diff ${OUT_FILE} ${OUT_FILE}.IS > /dev/null
result=$?

diff -u  ${OUT_FILE} ${OUT_FILE}.IS | tail +4 | sed s/^/">>>"/ | sed s/$/'\\n'/

rm -f ${OUT_FILE}.IS

exit ${result}

