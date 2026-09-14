#!/usr/bin/env sh

test_encrypt1() {
    output=$(./build/lab_1/lab1 -a "абвгдеёжзийклмнопрстуфхцчшщъыьэюя" -m encrypt -s 3 -g "привет" 2>/dev/null)
    if [ "$output" = "тулезх" ]; then
        echo 'ok'
    else
        echo "failed: 'тулезх' != '$output'"
        exit
    fi
}

cd "$(dirname "$0")"
if [ -e build/lab_1/lab1 ]
then
    test_encrypt1
else
    echo "Building executable"
    @make lab1
    test_encrypt1
fi
