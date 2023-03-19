#!/bin/bash

criterion_target=$1
cmake --build . --target $(basename ${criterion_target})

./${criterion_target} || exit 2
