clean:
	rm -rf build/

lab1: lab_1/main.cpp
	@mkdir -p build/lab_1
	gcc -Wall -Wextra lab_1/main.c -o build/lab_1/lab1
