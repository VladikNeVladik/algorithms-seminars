#-----------
# Toolchain
#-----------

CC  = x86_64-linux-gnu-gcc
CXX = g++
AS  = nasm
GDB = gdb

#-----------------------
# Compiler/linker flags
#-----------------------

CFLAGS = \
	-m32 \
	-fcf-protection=none \
	-fno-stack-protector \
	-fno-plt \
	-fno-pic \
	-no-pie \
	-I include \
	-O3 \
	-flto \
	-g

ASFLAGS = \
	-f elf32 \
	-DUNIX \
	-g

LDFLAGS = \
	-m32 \
	-fcf-protection=none \
	-fno-stack-protector \
	-fno-plt \
	-fno-pic \
	-no-pie \
	-z noexecstack \
	-O3 \
	-flto \
	-g

#-------
# Files
#-------

SOURCES     = src/main.c src/queue-asm.asm src/queue-c.c
OBJECTS1    = build/main.o build/queue-c.o
EXECUTABLE1 = build/linked_list_c
OBJECTS2    = build/main.o build/queue-asm.o
EXECUTABLE2 = build/linked_list_asm

#---------------
# Build scripts
#---------------

all: $(EXECUTABLE1) $(EXECUTABLE2) $(SOURCES)

$(EXECUTABLE1): $(OBJECTS1) Makefile
	@$(CC) $(LDFLAGS) $(OBJECTS1) -o $@

$(EXECUTABLE2): $(OBJECTS2) Makefile
	@$(CC) $(LDFLAGS) $(OBJECTS2) -o $@

build/%.o: src/%.c
	@mkdir -p build
	@$(CC) $(CFLAGS) -c $< -o $@

build/%.o: src/%.asm
	@mkdir -p build
	@$(AS) $(ASFLAGS) $< -o $@

#----------------------
# Emulator interaction
#----------------------

GDB_FLAGS = \
	--eval-command="set architecture i386" \
	--eval-command="file $(EXECUTABLE2)" \
	--eval-command="b main" \
	--eval-command="run" \
	--eval-command="tui e" \
	--eval-command="la regs"

gdb: $(EXECUTABLE)
	$(GDB) $(GDB_FLAGS)

.PHONY: all clean qemu gdb

# Подключаем тестовую инфраструктуру.
PROGRAM=apply
PROGRAM_NAME=37_linked_list_asm
TESTS = 00

include ../../homework/common.mk