#!/bin/sh

BASE_DIR=$(dirname $0)/../
BASE_DIR=$(cd $BASE_DIR;pwd)
CDEV_PRG=${BASE_DIR}/build/rdp_cdev
VALUE_FILE=${BASE_DIR}/tests/criterion/rdp_values_test/test_data_sorted.csv
PID=-1

[ ${UID} -ne 0 ] && {
    echo "Not running as root, restarting" >&2
    sudo $0 $*
} 


[ ${UID} -eq 0 ] && {
    [ -e ${VALUE_FILE} ] || {
        echo "ERROR: ${VALUE_FILE} not found" >&2
        exit 1
    }
    
    [ -e  ${CDEV_PRG} ] && {
        NOW=$(date "+%s")
        ${CDEV_PRG} --time ${NOW} --values ${VALUE_FILE} &
        PID=$!
        trap "echo The script is terminated killing; killall rdp_cdev; exit " SIGINT
        sleep 1
        chmod 666 /dev/rdp_cdev
        inotifywait  -r ${BASE_DIR} -e modify -e create -e delete -e move -e move_self
        killall rdp_cdev 
        [ $? -eq 130 ] && {
            echo "Exit because of interrupt" >&2
        }
        sleep 5
        . $0  

    }
}
# strange
