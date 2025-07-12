# Copyright 2024 Vladislav Aleinik

build/typecast: typecast.c
	@mkdir -p build
	@gcc typecast.c -Wall -Werror -o build/typecast

run: build/typecast
	@./build/typecast

.PHONY: run

# Подключаем тестовую инфраструктуру.
PROGRAM=typecast
PROGRAM_NAME=04_typecasting
TESTS = 00

include ../../homework/common.mk