# Copyright 2024 Vladislav Aleinik
# Наиспростейший возможный Makefile.

build/hello: hello.c
	@mkdir -p build
	@$(CC) hello.c -Wall -Werror -o build/hello

run: build/hello
	@./build/hello

python:
	@./hello.py

.PHONY: run

# Подключаем тестовую инфраструктуру.
PROGRAM=hello
PROGRAM_NAME=02_helloworld
TESTS = 00

include ../../homework/common.mk