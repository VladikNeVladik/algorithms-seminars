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

INCLUDES=\
	tree-avl.h \
	tree-rb.h \
	utils.h

build/test: test.c $(INCLUDES)
	@# Do nothing

tree-avl: test.c $(INCLUDES)
	@mkdir -p build
	@$(CC) test.c ${CFLAGS} -DTREE_AVL -o build/test

tree-rb: test.c $(INCLUDES)
	@mkdir -p build
	@$(CC) test.c ${CFLAGS} -DTREE_RB -o build/test

run: build/test
	@./build/test

.PHONY: run

# Подключаем тестовую инфраструктуру.
PROGRAM=test
PROGRAM_NAME=16_balanced_tree-$(FLAVOUR)
TESTS = 00

include ../../homework/common.mk