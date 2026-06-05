# Copyright 2026 Vladislav Aleinik

# Compilation flags:
CFLAGS=-O2 -Wall -Werror

build/test: test.c $(INCLUDES)
	@mkdir -p build
	@mkdir -p res
	@$(CC) test.c ${CFLAGS} -o build/test

run: build/test
	@./build/test

.PHONY: run

# Подключаем тестовую инфраструктуру.
PROGRAM=test
PROGRAM_NAME=12_stdio_buffering
TESTS = 00

include ../../homework/common.mk