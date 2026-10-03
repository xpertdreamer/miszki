#!/usr/bin/env bash

NAME=$1
arg=$2

ARG="0"
if [[ "$arg" ==  "-d" ]]; then
    ARG="1"
fi

make $NAME DOCS=$ARG
