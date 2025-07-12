# Copyright 2024 Vladislav Aleinik

build/operations: operations.c
	@mkdir -p build
	@gcc operations.c -Wall -Werror -o build/operations

run: build/operations
	@./build/operations

.PHONY: run

# Подключаем тестовую инфраструктуру.
PROGRAM=operations
PROGRAM_NAME=04_operations
TESTS = 00

include ../../homework/common.mk