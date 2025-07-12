# Copyright 2024 Vladislav Aleinik

CFLAGS = \
	-Wall \
	-Werror \
	-fno-stack-protector

build/password: password.c
	@mkdir -p build
	@gcc password.c $(CFLAGS) -g -o build/password

run: build/password
	@./build/password

# Очень странно, но без команды Make эти две команды возвращают разный результат
break: build/password
	printf 01234567890123456789012345678901_123456789ABCDE\\x00 | ./build/password
	printf 01234567890123456789012345678901_123456789ABCDE\\x01 | ./build/password

.PHONY: run

# Подключаем тестовую инфраструктуру.
PROGRAM=password
PROGRAM_NAME=09_password
TESTS = 00

include ../../homework/common.mk
