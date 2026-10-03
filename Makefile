DEBUG ?= 0
PKG ?= lab_3
DOCS ?= 0

clean:
	rm -rf build/

lab1: lab_1/main.c lab_1/substitution.c lab_1/input.c lab_1/replacement.c
	@mkdir -p build/lab_1
	gcc -Wall -Wextra $^ -o build/lab_1/lab1

lab2: lab_2/util/util.go lab_2/main.go lab_2/factorization/factor.go lab_2/primes/prime.go
	@mkdir -p build/lab_2
	cd lab_2 && go build -mod=vendor -ldflags "-X mizski/lab2/util.DebugMode=$(DEBUG)"  -o ../build/lab_2/lab2 .
	@if [ "$(DOCS)" == "1" ] || [ "$(DOCS)" == "true" ]; then \
		mkdir -p docs/html/ ; \
		cd lab_2 && doc2go -out ../docs/html/godoc ./... ; \
	fi



test:
	cd $(PKG) && go test -v ./... | grep -v 'Timer '
