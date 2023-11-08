#!/bin/bash
BASE_DIR=$(dirname $0)/../
BASE_DIR=$(cd $BASE_DIR;pwd)
CDEV_PRG=${BASE_DIR}/build/rdp_cdev
VALUE_FILE=${BASE_DIR}/tests/criterion/rdp_values_test/test_data_sorted.csv
PID=-1
LOG_LEVEL=2

[ -z "${RDP_CUSE_MAJOR}" ] && RDP_CUSE_MAJOR=243
[ -z "${RDP_CUSE_MINOR}" ] && RDP_CUSE_MINOR=0

log() {
  level=$1; shift;
  level_str="LVL ${level}"
  [ ${level} -eq 0 ] && level_str="   TRACE"
  [ ${level} -eq 1 ] && level_str="   DEBUG"
  [ ${level} -eq 2 ] && level_str="    INFO"
  [ ${level} -eq 3 ] && level_str=" WARNING"
  [ ${level} -eq 4 ] && level_str="   ERROR"
  [ ${level} -eq 5 ] && level_str="CRITICAL"

  if [ ${level} -ge ${LOG_LEVEL} ]; then
    printf "%s: %s\n" ${level_str} "$*" >&2
  fi
}

fail() {
  exit_code=$1; shift
  log 5 ERROR $*
  exit ${exit_code}
}
        
[ "${UID}" -ne "0" ] && {
    log 3 "Not running as root, restarting"
    sudo RDP_CUSE_MAJOR=${RDP_CUSE_MAJOR} RDP_CUSE_MINOR=${RDP_CUSE_MINOR} $0 $*
} 

[ "${UID}" -eq "0" ] && {
    [ -e ${VALUE_FILE} ] || {
        fail 1 "${VALUE_FILE} not found"
    }
    [ -e  ${CDEV_PRG} ] && {
        NOW=$(date "+%s")
        log 2 RDP_CUSE_MINOR is ${RDP_CUSE_MINOR}
        echo ${CDEV_PRG} --time ${NOW} --values ${VALUE_FILE} --major ${RDP_CUSE_MAJOR} --minor ${RDP_CUSE_MINOR} & 
        ${CDEV_PRG} --time ${NOW} --values ${VALUE_FILE} --major ${RDP_CUSE_MAJOR} --minor ${RDP_CUSE_MINOR} & 
        PID=$!
        trap "echo The script is terminated killing; killall rdp_cdev; exit " SIGINT
        log 2 "Script is running with pid ${PID}"
        sleep 1
        inotifywait  -r ${BASE_DIR} -e modify -e create -e delete -e move -e move_self
        log 3 kill all remaining rdp_cdev
        killall rdp_cdev 
        [ $? -eq 130 ] && {
            log 3 "Exit because of interrupt" >&2
        }
        sleep 5
        . $0  
    }
}
