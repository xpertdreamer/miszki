clean:
	rm -rf build/

lab1: lab_1/main.c lab_1/substitution.c lab_1/input.c
	@mkdir -p build/lab_1
	gcc -Wall -Wextra $^ -o build/lab_1/lab1
