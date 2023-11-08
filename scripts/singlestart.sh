#!/bin/sh

BASE_DIR=$(cd $(dirname $0/)/../; pwd)
RDP_CUSE_MAJOR=244
RDP_CUSE_MINOR=4
VALUE_FILE=${BASE_DIR}/tests/criterion/rdp_values_test/test_data_sorted.csv

[ -n "$1" ] && RDP_CUSE_MAJOR=$1
[ -n "$2" ] && RDP_CUSE_MINOR=$2

NOW=$(date "+%s")
${BASE_DIR}/build/rdp_cdev --time ${NOW} --values ${VALUE_FILE} --major ${RDP_CUSE_MAJOR} --minor ${RDP_CUSE_MINOR}  
