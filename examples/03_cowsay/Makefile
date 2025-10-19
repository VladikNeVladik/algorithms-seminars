# Copyright 2024 Vladislav Aleinik

build/cowsay: cowsay.c
	@mkdir -p build
	@$(CC) cowsay.c -std=c99 -Wall -Werror -o build/cowsay

run: build/cowsay
	@./build/cowsay

.PHONY: run

# Подключаем тестовую инфраструктуру.
PROGRAM=cowsay
PROGRAM_NAME=03_cowsay
TESTS = 00

include ../../homework/common.mk