# Copyright 2024 Vladislav Aleinik

build/square: square.c
	@mkdir -p build
	@$(CC) square.c -Wall -Werror -o build/square -lm

run: build/square
	@./build/square

.PHONY: run

# Подключаем тестовую инфраструктуру.
PROGRAM=square
PROGRAM_NAME=06_square_equation
TESTS = 00

include ../../homework/common.mk