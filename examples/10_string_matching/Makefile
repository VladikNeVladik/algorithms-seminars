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

WAR_AND_PEACE_URL="https://gist.githubusercontent.com/romaklimenko/c95f3a864828f7f034b7a33d1676e420/raw/55f9027799b5b3c67e2f7cb3d6a7154f707ff08a/warandpeace.txt"
HAYSTACK_FILE=res/warandpeace.txt

build/search: search.c $(HAYSTACK_FILE)
	@mkdir -p build
	@$(CC) search.c ${CFLAGS} -o build/search

$(HAYSTACK_FILE):
	@mkdir -p res
	@cd res && wget $(WAR_AND_PEACE_URL)

install-res: $(HAYSTACK_FILE)

run: build/search
	@./build/search

.PHONY: run install-res

# Подключаем тестовую инфраструктуру.
PROGRAM=search
PROGRAM_NAME=10_string_matching
TESTS = 00

include ../../homework/common.mk
