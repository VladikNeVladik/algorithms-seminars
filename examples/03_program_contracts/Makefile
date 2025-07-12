# Copyright 2024 Vladislav Aleinik

build/adder: adder.c
	@mkdir -p build
	@gcc adder.c -std=c99 -Wall -Werror -o build/adder

run: build/adder
	@./build/adder

.PHONY: run

# Подключаем тестовую инфраструктуру.
PROGRAM=adder
PROGRAM_NAME=03_program_contracts
TESTS = 00

include ../../homework/common.mk