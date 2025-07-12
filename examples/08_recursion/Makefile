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
	-std=gnu99                   \
	-lm

build/fibs: fibs.c
	@# Do nothing

# Different build scenarios:
cycle: fibs.c
	@mkdir -p build
	@gcc fibs.c ${CFLAGS} -DFIBS_CYCLE -o build/fibs

bad: fibs.c
	@mkdir -p build
	@gcc fibs.c ${CFLAGS} -DFIBS_BAD_RECURSION -o build/fibs

good: fibs.c
	@mkdir -p build
	@gcc fibs.c ${CFLAGS} -DFIBS_GOOD_RECURSION -o build/fibs

# Run scenarios:
run:
	@./build/fibs

.PHONY: run

# Подключаем тестовую инфраструктуру.
PROGRAM=fibs
PROGRAM_NAME=08_recursion-$(FLAVOUR)
TESTS = 00

include ../../homework/common.mk