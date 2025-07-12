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
	matrix.h \
	vector.h \
	utils.h

build/test: test.c $(INCLUDES)
	@mkdir -p build
	@mkdir -p res
	@gcc test.c ${CFLAGS} -o build/test

run: build/test
	@./build/test

.PHONY: run

# Подключаем тестовую инфраструктуру.
PROGRAM=test
PROGRAM_NAME=12_matrix_fileio
TESTS = 00

include ../../homework/common.mk