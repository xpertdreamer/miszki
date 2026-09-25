DEBUG ?= false

clean:
	rm -rf build/

lab1: lab_1/main.c lab_1/substitution.c lab_1/input.c lab_1/replacement.c
	@mkdir -p build/lab_1
	gcc -Wall -Wextra $^ -o build/lab_1/lab1

lab2: lab_2/util/util.go lab_2/main.go
	@mkdir -p build/lab_2
	cd lab_2 && go build -ldflags "-X mizski/lab2/util.DebugMode=$(DEBUG)"  -o ../build/lab_2/lab2 .
