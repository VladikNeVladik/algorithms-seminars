# Copyright 2024 Vladislav Aleinik

# Compilation flags:
CFLAGS=\
	-O2                          \
	-Wall                        \
	-Werror                      \
	-Wno-unknown-warning-option  \
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

build/sum: sum.c
	@# Do nothing

# Different build scenarios:
simple: sum.c
	@mkdir -p build
	@$(CC) sum.c ${CFLAGS} -DSUM_SIMPLE -o build/sum

generator: sum.c
	@mkdir -p build
	@$(CC) sum.c ${CFLAGS} -DSUM_GENERATOR -o build/sum

tail: sum.c
	@mkdir -p build
	@$(CC) sum.c ${CFLAGS} -DSUM_TAIL_RECURSION -o build/sum

cycle: sum.c
	@mkdir -p build
	@$(CC) sum.c ${CFLAGS} -DSUM_CYCLE -o build/sum

# Run scenarios:
run:
	@./build/sum

.PHONY: run

# Подключаем тестовую инфраструктуру.
PROGRAM=sum
PROGRAM_NAME=08_generators-$(FLAVOUR)
TESTS = 00

include ../../homework/common.mk
