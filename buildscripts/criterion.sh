#!/bin/bash

criterion_target=$1

make $1 || exit 1
./$1 || exit 2
