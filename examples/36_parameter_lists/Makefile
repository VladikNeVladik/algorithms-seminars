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
	-Wfatal-errors \
	-no-pie \
	-g

CXXFLAGS = \
	$(CFLAGS) \
	--std=c++14

ASFLAGS = \
	-f elf32 \
	-DUNIX \
	-g

LDFLAGS = \
	-m32 \
	-no-pie \
	-z noexecstack \
	-g

#-------
# Files
#-------

SOURCES    = src/main.asm
OBJECTS    = build/main.o
EXECUTABLE = build/apply

SOURCE_EQUIV     = src/equivalent.cpp
OBJECTS_EQUIV    = build/equivalent.o
EXECUTABLE_EQUIV = build/equivalent

#---------------
# Build scripts
#---------------

all: $(EXECUTABLE) $(EXECUTABLE_EQUIV) $(SOURCES)

$(EXECUTABLE): $(OBJECTS) Makefile
	@$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(EXECUTABLE_EQUIV): $(OBJECTS_EQUIV) Makefile
	@$(CXX) $(CXXFLAGS) $< -o $@

build/%.o: src/%.cpp
	@mkdir -p build
	@$(CXX) $(CXXFLAGS) -c $< -o $@

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
	--eval-command="file $(EXECUTABLE)" \
	--eval-command="b main" \
	--eval-command="run" \
	--eval-command="tui e" \
	--eval-command="la regs"

gdb: $(EXECUTABLE)
	$(GDB) $(GDB_FLAGS)

.PHONY: all clean qemu gdb

# Подключаем тестовую инфраструктуру.
PROGRAM=apply
PROGRAM_NAME=35_parameter_lists
TESTS = 00

include ../../homework/common.mk