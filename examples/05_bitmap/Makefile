# Copyright 2024 Vladislav Aleinik

build/bitmap: bitmap.c
	@mkdir -p build
	@$(CC) bitmap.c -Wall -Werror -o build/bitmap

run: build/bitmap
	@./build/bitmap

.PHONY: run

# Подключаем тестовую инфраструктуру.
PROGRAM=bitmap
PROGRAM_NAME=05_bitmap
TESTS = 00

include ../../homework/common.mk