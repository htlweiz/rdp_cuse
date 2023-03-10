#!/bin/bash

cmake --build .. --target criterion_echo

make $1 || exit 1
./$1 || exit 2
