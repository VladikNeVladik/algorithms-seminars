# Copyright 2024 Vladislav Aleinik

build/sort: sort.c
	@mkdir -p build
	@$(CC) sort.c -Wall -Werror -o build/sort

run: build/sort
	@./build/sort

.PHONY: run

# Подключаем тестовую инфраструктуру.
PROGRAM=sort
PROGRAM_NAME=06_sort
TESTS = 00

include ../../homework/common.mk