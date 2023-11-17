#!/bin/sh

BASE_DIR=$(dirname $0)/../
BASE_DIR=$(cd $BASE_DIR;pwd)
CDEV_PRG=${BASE_DIR}/build/rdp_cdev
VALUE_FILE=${BASE_DIR}/tests/criterion/rdp_values_test/test_data_sorted.csv
[ -z "${RDP_CUSE_MAJOR}" ] && RDP_CUSE_MAJOR=243
[ -z "${RDP_CUSE_MINOR}" ] && RDP_CUSE_MINOR=0

NOW=$(date "+%s")

${CDEV_PRG} --time ${NOW} --values ${VALUE_FILE} --major ${RDP_CUSE_MAJOR} --minor ${RDP_CUSE_MINOR} & 
pid=$!

RUNNING=true

trap "Bye!; RUNNING=false; kill ${pid}; sleep 2; kill -9 ${pid}" SIGINT

while ${RUNNING}; do
  sleep 1
done

