# Copyright 2024 Vladislav Aleinik

# Compilation flags:
CFLAGS=\
	-O2                          \
	-Wall                        \
	-Werror                      \
	-Wformat-security            \
	-Wignored-qualifiers         \
	-Winit-self                  \
	-Wswitch-default             \
	-Wfloat-equal                \
	-Wpointer-arith              \
	-Wtype-limits                \
	-Wempty-body                 \
	-Wno-logical-op              \
	-Wstrict-prototypes          \
	-Wold-style-declaration      \
	-Wold-style-definition       \
	-Wmissing-parameter-type     \
	-Wmissing-field-initializers \
	-Wnested-externs             \
	-Wno-pointer-sign            \
	-Wno-unused-result           \
	-Wno-maybe-uninitialized     \
	-std=gnu99                   \
	-lm

INCLUDES=\
	database.h \
	utils.h

build/create: create.c $(INCLUDES)
	@mkdir -p build res
	@$(CC) create.c ${CFLAGS} -o build/create

build/search: search.c $(INCLUDES)
	@mkdir -p build
	@$(CC) search.c ${CFLAGS} -o build/search

create: build/create
	@mkdir -p res
	@./build/create

search: build/search
	@mkdir -p res
	@./build/search

.PHONY: create search clean

# Подключаем тестовую инфраструктуру.
PROGRAM=search
PROGRAM_NAME=13_database
TESTS = 00

include ../../homework/common.mk