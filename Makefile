DEBUG ?= 0
PKG ?= lab_2
DOCS ?= 0

define run_doxygen
	@if [ "$(DOCS)" = "1" ]; then \
		doxygen; \
	fi
endef

clean:
	rm -rf build/

lab1: lab_1/main.c lab_1/substitution.c lab_1/input.c lab_1/replacement.c
	@mkdir -p build/lab_1
	gcc -Wall -Wextra $^ -o build/lab_1/lab1
	$(call run_doxygen)

lab2: lab_2/util/util.go lab_2/main.go lab_2/factorization/factor.go lab_2/primes/prime.go
	@mkdir -p build/lab_2
	cd lab_2 && go build -mod=vendor -ldflags "-X mizski/lab2/util.DebugMode=$(DEBUG)"  -o ../build/lab_2/lab2 .
	@if [ "$(DOCS)" == "1" ] || [ "$(DOCS)" == "true" ]; then \
		mkdir -p docs/html/ ; \
		cd lab_2 && doc2go -out ../docs/html/godoc ./... ; \
	fi

lab3: lab_3/main.cpp lab_3/math.cpp
	@mkdir -p build/lab_3
	g++ -Wall -Wextra $^ -o build/lab_3/lab3 -DDEBUG_MODE=$(DEBUG)
	$(call run_doxygen)

test:
	cd $(PKG) && go test -v ./... | grep -v 'Timer '
