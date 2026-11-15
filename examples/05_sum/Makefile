# Copyright 2024 Vladislav Aleinik

build/sum: sum.c
	@mkdir -p build
	@$(CC) sum.c -Wall -Werror -o build/sum

run: build/sum
	@./build/sum

.PHONY: run

# Подключаем тестовую инфраструктуру.
PROGRAM=sum
PROGRAM_NAME=05_sum
TESTS = 00

include ../../homework/common.mk