#!/usr/bin/env bash

cd "$(dirname "$0")"

NAME=$1
DIR="${NAME:0:3}"_"${NAME:3:1}"
echo "$DIR"

./build/"$DIR"/"$NAME"
