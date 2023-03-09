#!/bin/sh

usage() {
  echo "usage: $(basename $0) [-c] -[C] [-v]"
  echo "    -c clean and cmake"
  echo "    -C cmake"
  echo "    -v verbose"
  echo "    -h help"
}

taillog() {
  if [ -n "$1" ]; then
    echo
    echo --------------------------------------------------------------------------------
    echo LOGs
    echo --------------------------------------------------------------------------------
    echo
    awk -f ../tests/test.awk Testing/Temporary/LastTest.log  
    echo
    echo --------------------------------------------------------------------------------
    echo Following Tests failed.
    echo --------------------------------------------------------------------------------
    cat Testing/Temporary/LastTestsFailed.log  
  else
    echo Error occured rerun with -v to show log
  fi
}

main() {
  CLEAN=""
  CMAKE=""
  VERBOSE=""
  while [ -n "$1" ]; do
    if [ "$1" == "-c" ]; then
      echo "got -c"
      CLEAN=Y
      CMAKE=Y
      shift
    elif [ "$1" == "-C" ]; then
      echo "got -C"
      CMAKE=Y
      shift
    elif [ "$1" == "-v" ]; then
      echo "got -v"
      VERBOSE=Y
      shift
    elif [ "$1" == "-h" ]; then
      usage
      exit 0
    else
      echo Error $1 unknown
      usage
      exit 1
    fi
  done

  cd $(dirname $0)
  echo WARNING: clean \"${CLEAN}\"
  if [ -n "${CLEAN}" ]; then
    echo WARNING: running clean build
    rm -rf build
  fi
  mkdir -p build
  cd build
  if [ -n "${CMAKE}" ]; then
    echo INFO: rerunning cmake
    cmake ..
  fi
  make -j 4 && make test || taillog $VERBOSE
}

main $@
