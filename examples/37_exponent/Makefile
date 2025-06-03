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
	-no-pie \
	-g \

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

SOURCES = \
	src/main.asm

OBJECTS_HALFWAY_DONE = $(SOURCES:src/%.c=build/%.o)
OBJECTS              = $(OBJECTS_HALFWAY_DONE:src/%.asm=build/%.o)

EXECUTABLE = build/x87-matrix-mul

#---------------
# Build scripts
#---------------

all: $(EXECUTABLE) $(SOURCES)

$(EXECUTABLE): $(OBJECTS) Makefile
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ -lm

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: src/%.asm
	@mkdir -p build
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf build

run: $(EXECUTABLE)
	./$(EXECUTABLE)

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
