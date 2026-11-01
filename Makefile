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

shell: lab_5/shell/shell.c
	@mkdir -p build/lab_5/obj
	gcc -Wall -Wextra -Ilab_5/shell -DDEBUG_MODE=$(DEBUG) \
		-g -c $^ \
		-o build/lab_5/obj/shell.o

	ar rvs build/lab_5/shell.a \
		build/lab_5/obj/shell.o

SPATH ?= $(HOME)/secret/key
SLEN ?= 32

crypt:
	@echo
	@echo 'Run `openssl rand -hex 32 > ~/path/file` to generate salt'
	@echo
	@mkdir -p build/lab_5
	gcc -Wall -Wextra -Ilab_5/crypt -fPIC -shared -lcrypto \
		lab_5/crypt/crypt.c \
		-DSALTPATH=\"$(SPATH)\" \
		-DSALTLEN=$(SLEN) \
		-o build/lab_5/libmycrypt.so

CRTARGET ?= create
ACTARGET ?= account
LIB ?= ./build/lab_5/libmycrypt.so

lab5: shell crypt
	@cp lab_5/shell/shell.h lab_5/
	@cp	lab_5/shell/shell.h lab_5/create/
	@cp lab_5/shell/shell.h lab_5/account

	gcc -Wall -Wextra -Ilab_5 -DDEBUG_MODE=$(DEBUG) -DEXECUTABLE_NAME=\"$(ACTARGET)\" \
		-DLIB=\"$(LIB)\" \
		-g lab_5/account/account.c lab_5/account/log.c lab_5/account/util.c lab_5/account/handler.c\
		-o build/lab_5/$(ACTARGET) \
		-Lbuild/lab_5 -l:shell.a \
		-ldl

	gcc -Wall -Wextra -Ilab_5 -DDEBUG_MODE=$(DEBUG) -DEXECUTABLE_NAME=\"$(CRTARGET)\" \
		-DLIB=\"$(LIB)\" \
		-g lab_5/create/main.c lab_5/create/create.c lab_5/create/log.c lab_5/create/util.c \
		-o build/lab_5/$(CRTARGET) \
		-Lbuild/lab_5 -l:shell.a \
		-ldl

	gcc -Wall -Wextra -Ilab_5 -DDEBUG_MODE=$(DEBUG) \
		-g lab_5/entrypoint.c \
		-Lbuild/lab_5 -l:shell.a \
		-o build/lab_5/lab5

test:
	cd $(PKG) && go test -v ./... | grep -v 'Timer '
