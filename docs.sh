#!/usr/bin/env bash

ARG=$1

cd "$(dirname "$0")"

case "$ARG" in
    "lab1")
        xdg-open ./docs/html/dir_72a8e3e02ea77f359f7bbb8cd8558b60.html
        ;;
    "lab2")
        xdg-open ./docs/html/godoc/index.html
        ;;
    "lab3")
        xdg-open ./docs/html/dir_275ea8e33b25bc38d30390361bf16bcb.html
        ;;
    *)
        echo "Unknown name: $ARG"
        echo "Usage: $0 [lab1|lab2|lab3]"
        exit 1
        ;;
esac
