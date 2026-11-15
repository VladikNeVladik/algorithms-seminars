# Copyright 2024 Vladislav Aleinik

build/2dsum: 2dsum.c
	@mkdir -p build
	@$(CC) 2dsum.c -Wall -Werror -o build/2dsum

run: build/2dsum
	@./build/2dsum

.PHONY: run

# Подключаем тестовую инфраструктуру.
PROGRAM=2dsum
PROGRAM_NAME=05_2d_array
TESTS = 00

include ../../homework/common.mk