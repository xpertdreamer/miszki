#!/usr/bin/env bash

NAME=$1
shift

ARG="0"
ARG2="0"

while [[ $# -gt 0 ]]; do
    case "$1" in
        -d)     ARG="1" ;;
        -debug) ARG2="1" ;;
    esac
    shift
done

make "$NAME" DOCS="$ARG" DEBUG="$ARG2"
