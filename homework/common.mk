# Copyright 2024 Vladislav Aleinik

#=====================#
# Compilation process #
#=====================#

ifndef CC
CC=gcc
endif

# Compilation flags:
CFLAGS_TEST = \
	-O2                          \
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

ifndef NDEBUG
CFLAGS_TEST += -g
endif

# The fun stuff:
ifndef NOSAN
CFLAGS_TEST += \
	-fsanitize=address    \
	-fsanitize=leak       \
	-fsanitize=undefined  \
	-fstack-protector-all \
	-U_FORTIFY_SOURCE     \
	-D_FORTIFY_SOURCE=0   \
	-ftrivial-auto-var-init=pattern
endif

#========#
# Colors #
#========#

BRED    = \033[1;31m
BGREEN  = \033[1;32m
BYELLOW = \033[1;33m
GREEN   = \033[1;35m
BCYAN   = \033[1;36m
RESET   = \033[0m

#=======================#
# Hand-made test system #
#=======================#

# Timing command usage:
TIME_CMD    = /usr/bin/time
TIME_FORMAT = \
	"CPU Percentage: %P\nReal time: %e sec\nUser time: %U sec\nMemory: %t kB\nReturn value: %x"

#===================#
# Command arguments #
#===================#

PROGRAM_BIN=build/$(PROGRAM)

default: verify-input-program $(PROGRAM_BIN)

# Command arguments:
# PROGRAM - program being tested
# MAX_MEMORY - memory limit for heap
ifndef MAX_MEMORY
MAX_MEMORY=64000000# 64MiB
endif

ifndef PROGRAM_NAME
PROGRAM_NAME=$(PROGRAM)
endif

verify-input-program:
	@if [ -z "$(PROGRAM)" ]; then \
		printf "$(BRED)Error$(BYELLOW): define program as $(BCYAN)PROGRAM=program$(RESET)\n"; \
		exit 1; \
	fi

#=======================#
# Hand-made test system #
#=======================#

# Test verdicts:
VERDICT_INAPT = "$(BYELLOW)[INAPT]$(RESET)\n"
VERDICT_OK    = "$(BGREEN)[OK]$(RESET)\n"
VERDICT_ERR   = "$(BRED)[ERR]$(RESET)\n"

OUTPUTS_TEST   = $(TESTS:%=build/%.ans.test)
OUTPUTS_TIME   = $(TESTS:%=build/%.ans.time)
OUTPUTS_MEMORY = $(TESTS:%=build/%.ans.memory)
OUTPUTS_IO_BIN = $(TESTS:%=build/%.ans.io-bin)
OUTPUTS_BIN_IO = $(TESTS:%=build/%.ans.bin-io)

test:        verify-input-program $(PROGRAM_BIN) $(OUTPUTS_TEST)   FORCE
time:        verify-input-program $(PROGRAM_BIN) $(OUTPUTS_TIME)   FORCE
memory:      verify-input-program $(PROGRAM_BIN) $(OUTPUTS_MEMORY) FORCE
test-io-bin: verify-input-program $(PROGRAM_BIN) $(OUTPUTS_IO_BIN) FORCE
test-bin-io: verify-input-program $(PROGRAM_BIN) $(OUTPUTS_BIN_IO) FORCE

build/%.ans.test: tests/%.dat $(PROGRAM_BIN) FORCE
	@mkdir -p build
	@printf "$(BYELLOW)Run test $(BCYAN)$*$(BYELLOW) for solution $(BCYAN)$(PROGRAM_NAME)$(RESET)\n"
	@$(PROGRAM_BIN) < $< > $@ | cat
	@if [ ! -f tests/$*.ans ]; then \
		printf $(VERDICT_INAPT); \
	elif cmp -s tests/$*.ans $@; then \
		printf $(VERDICT_OK); \
	else \
		printf $(VERDICT_ERR); \
	fi

build/%.ans.time: tests/%.dat $(PROGRAM_BIN) FORCE
	@mkdir -p build
	@printf "$(BYELLOW)Measure time on $(BCYAN)$*$(BYELLOW) for solution $(BCYAN)$(PROGRAM_NAME)$(RESET)\n"
	@$(TIME_CMD) --quiet --format=$(TIME_FORMAT) $(PROGRAM_BIN) < $< > $@ | cat
	@if [ ! -f tests/$*.ans ]; then \
		printf $(VERDICT_INAPT); \
	elif cmp -s tests/$*.ans $@; then \
		printf $(VERDICT_OK); \
	else \
		printf $(VERDICT_ERR); \
	fi

build/%.ans.memory: tests/%.dat $(PROGRAM_BIN) FORCE
	@mkdir -p build
	@printf "$(BYELLOW)Limit memory ($(BCYAN)$(MAX_MEMORY)$(BYELLOW))"\
	" on $(BCYAN)$*$(BYELLOW) for \033[1;36m$(PROGRAM_NAME)$(RESET)\n"
	@prlimit --data=$(MAX_MEMORY) $(PROGRAM_BIN) < $< > $@ | cat
	@if [ ! -f tests/$*.ans ]; then \
		printf $(VERDICT_INAPT); \
	elif cmp -s tests/$*.ans $@; then \
		printf $(VERDICT_OK); \
	else \
		printf $(VERDICT_ERR); \
	fi

build/%.ans.io-bin: tests/%.dat $(PROGRAM_BIN) FORCE
	@mkdir -p build
	@printf "$(BYELLOW)Run test $(BCYAN)$*$(BYELLOW) for solution $(BCYAN)$(PROGRAM_NAME)$(RESET)\n"
	@$(PROGRAM_BIN) < $< | cat
	@mv output.bin $@
	@if [ ! -f tests/$*.ans ]; then \
		printf $(VERDICT_INAPT); \
	elif cmp -s tests/$*.ans $@; then \
		printf $(VERDICT_OK); \
	else \
		printf $(VERDICT_ERR); \
	fi

build/%.ans.bin-io: tests/%.dat $(PROGRAM_BIN) FORCE
	@mkdir -p build
	@printf "$(BYELLOW)Run test $(BCYAN)$*$(BYELLOW) for solution $(BCYAN)$(PROGRAM_NAME)$(RESET)\n"
	@cp tests/$*.dat input.bin
	@$(PROGRAM_BIN) > $@ | cat
	@rm input.bin
	@if [ ! -f tests/$*.ans ]; then \
		printf $(VERDICT_INAPT); \
	elif cmp -s tests/$*.ans $@; then \
		printf $(VERDICT_OK); \
	else \
		printf $(VERDICT_ERR); \
	fi

#==============#
# Build system #
#==============#

build/%: %.c FORCE
	@mkdir -p build
	@printf "$(BYELLOW)Building solution \033[1;36m$*$(RESET)\n"
	@$(CC) $< $(CFLAGS_TEST) -o $@

clean: FORCE
	@rm -rf build
	@rm -f tests/*-gen.dat

#================================#
# Contest manipulation utilities #
#================================#

clean-all: FORCE
	@rm -rf ../../contest-*/problem-*/build
	@rm  -f ../../contest-*/problem-*/tests/*-gen.dat

.PHONY: test clean FORCE
