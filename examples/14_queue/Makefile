# Copyright 2024 Vladislav Aleinik

# Compilation flags:
CFLAGS=\
	-O2                          \
	-Wall                        \
	-Werror                      \
	-Wformat-security            \
	-Wignored-qualifiers         \
	-Winit-self                  \
	-Wswitch-default             \
	-Wfloat-equal                \
	-Wpointer-arith              \
	-Wtype-limits                \
	-Wempty-body                 \
	-Wno-logical-op              \
	-Wstrict-prototypes          \
	-Wold-style-declaration      \
	-Wold-style-definition       \
	-Wmissing-parameter-type     \
	-Wmissing-field-initializers \
	-Wnested-externs             \
	-Wno-pointer-sign            \
	-Wno-unused-result           \
	-Wno-maybe-uninitialized     \
	-std=gnu99                   \
	-lm

INCLUDES=\
	queue.h \
	utils.h

build/requestlist: requestlist.c $(INCLUDES)
	@mkdir -p build res
	@gcc requestlist.c ${CFLAGS} -o build/requestlist

test: build/requestlist
	@./build/requestlist

.PHONY: create search

# Подключаем тестовую инфраструктуру.
include ../../homework/common.mk