#===========#
# Toolchain #
#===========#

CC  = x86_64-linux-gnu-gcc
AS  = nasm
AR  = ar
GDB = gdb

#=======================#
# Compiler/linker flags #
#=======================#

CFLAGS = \
	-m32 \
	-Wfatal-errors \
	-std=gnu99 \
	-fno-pie \
	-g

ASFLAGS = \
	-f elf32 \
	-DUNIX \
	-g

LDFLAGS = \
	-m32 \
	-fno-pie \
	-z noexecstack \
	-g

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

verify-input-program:
	@if [ -z "$(PROGRAM)" ]; then \
		printf "$(BRED)Error$(BYELLOW): define program as $(BCYAN)PROGRAM=program$(RESET)\n"; \
		exit 1; \
	fi

#=======#
# Files #
#=======#

LIBMAIN_SOURCES = \
	../../../macro-control.c \
	main.asm

LIBMAIN_OBJECTS = \
	build/macro-control.o \
	build/main.o

LIBMAIN = \
	build/libmain.a

EXEC_SOURCES = \
	$(PROGRAM).asm

EXEC_OBJECTS = \
	build/$(PROGRAM).o

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

test:   verify-input-program $(PROGRAM_BIN) $(OUTPUTS_TEST)   FORCE
time:   verify-input-program $(PROGRAM_BIN) $(OUTPUTS_TIME)   FORCE
memory: verify-input-program $(PROGRAM_BIN) $(OUTPUTS_MEMORY) FORCE

build/%.ans.test: tests/%.dat $(PROGRAM_BIN) FORCE
	@mkdir -p build
	@printf "$(BYELLOW)Run test $(BCYAN)$*$(BYELLOW) for solution $(BCYAN)$(PROGRAM)$(RESET)\n"
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
	@printf "$(BYELLOW)Measure time on $(BCYAN)$*$(BYELLOW) for solution $(BCYAN)$(PROGRAM)$(RESET)\n"
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
	" on $(BCYAN)$*$(BYELLOW) for \033[1;36m$(PROGRAM)$(RESET)\n"
	@prlimit --data=$(MAX_MEMORY) $(PROGRAM_BIN) < $< > $@ | cat
	@if [ ! -f tests/$*.ans ]; then \
		printf $(VERDICT_INAPT); \
	elif cmp -s tests/$*.ans $@; then \
		printf $(VERDICT_OK); \
	else \
		printf $(VERDICT_ERR); \
	fi

#---------------
# Build scripts
#---------------

build/macro-control.o: ../../../macro-control.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: %.asm
	@mkdir -p build
	$(AS) $(ASFLAGS) $< -o $@

$(LIBMAIN): $(LIBMAIN_OBJECTS)
	ar r $@ $^

$(PROGRAM_BIN): $(EXEC_OBJECTS) $(LIBMAIN) Makefile
	$(CC) $(LDFLAGS) $(EXEC_OBJECTS) $(LIBMAIN) -o $@

clean:
	rm -rf build

#----------------------
# Emulator interaction
#----------------------

GDB_FLAGS = \
	--eval-command="set architecture i386" \
	--eval-command="file $(PROGRAM_BIN)" \
	--eval-command="b main" \
	--eval-command="run" \
	--eval-command="tui e" \
	--eval-command="la regs"

gdb: $(PROGRAM_BIN)
	$(GDB) $(GDB_FLAGS)

.PHONY: all clean gdb FORCE
