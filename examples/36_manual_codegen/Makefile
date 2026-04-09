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
	-I include

OBJDUMP_FLAGS = 		\
	-M intel 			\
	--section=.text  	\
	--no-show-raw-insn

#---------------
# Build scripts
#---------------

tree-ops:
	@mkdir -p build
	@$(CC) $(CFLAGS) src/tree-ops.c -o build/tree-ops
	@objdump build/tree-ops -d $(OBJDUMP_FLAGS) > build/tree-ops.asm
	@$(CC) -S $(CFLAGS) -masm=intel src/tree-ops.c -o build/tree-ops-another.asm

include ../../homework/common.mk
