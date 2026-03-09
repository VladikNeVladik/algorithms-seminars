#-----------
# Toolchain
#-----------

CC  = x86_64-linux-gnu-gcc
AS  = nasm
GDB = gdb

#-----------------------
# Compiler/linker flags
#-----------------------

CFLAGS = \
	-m32 \
	-Wfatal-errors \
	-fcf-protection=none \
	-fno-stack-protector \
	-fno-plt \
	-fno-pic \
	-no-pie \
	-g

LDFLAGS = \
	-m32 \
	-no-pie \
	-fno-plt \
	-fno-pic \
	-z noexecstack \
	-g

#-------
# Files
#-------

SOURCES = \
	src/benchmark.c

EXECUTABLE = build/calling_convention

#---------------
# Build scripts
#---------------

all: $(EXECUTABLE) $(SOURCES)

$(EXECUTABLE): $(SOURCES) Makefile
	@mkdir -p build
	@$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCES) -o $@

#----------------------
# Emulator interaction
#----------------------

GDB_FLAGS = \
	--eval-command="set architecture i386" \
	--eval-command="file $(EXECUTABLE)" \
	--eval-command="b main" \
	--eval-command="run" \
	--eval-command="tui e" \
	--eval-command="la regs"

gdb: $(EXECUTABLE)
	$(GDB) $(GDB_FLAGS)

.PHONY: all clean qemu gdb

# Подключаем тестовую инфраструктуру.
PROGRAM=call
PROGRAM_NAME=34_calling_convention
TESTS = 00

include ../../homework/common.mk