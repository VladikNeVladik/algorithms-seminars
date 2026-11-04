# Copyright 2024 Vladislav Aleinik
# Наиспростейший возможный Makefile.

build/grademe: grademe.c
	@mkdir -p build
	@$(CC) grademe.c -Wall -Werror -o build/grademe -lm

run: build/grademe
	@./build/grademe

.PHONY: run

# Подключаем тестовую инфраструктуру.
PROGRAM=grademe
PROGRAM_NAME=02_grademe
TESTS = 00

include ../../homework/common.mk
