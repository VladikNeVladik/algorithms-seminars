#--------------------
# Набор инструментов
#--------------------

CC  = x86_64-linux-gnu-gcc
AS  = nasm
GDB = gdb

#----------------------------------
# Флаги компилятора и компоновщика
#----------------------------------

CFLAGS = \
	-m32 \
	-Wfatal-errors \
	-no-pie \
	-g

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
# Файлы
#-------

SOURCES = \
	src/benchmark.c \
	src/macro.c \
	src/main.asm

OBJECTS_HALFWAY_DONE = $(SOURCES:src/%.c=build/%.o)
OBJECTS              = $(OBJECTS_HALFWAY_DONE:src/%.asm=build/%.o)

EXECUTABLE = build/64bit_arithmetics

#----------------
# Скрипты сборки
#----------------

all: $(EXECUTABLE) $(SOURCES)

$(EXECUTABLE): $(OBJECTS) Makefile
	@$(CC) $(LDFLAGS) $(OBJECTS) -o $@

build/%.o: src/%.c
	@mkdir -p build
	@$(CC) $(CFLAGS) -c $< -o $@

build/%.o: src/%.asm
	@mkdir -p build
	@$(AS) $(ASFLAGS) $< -o $@

run: $(EXECUTABLE)
	./$(EXECUTABLE)

#------------------------
# Установка зависимостей
#------------------------

install-deps:
	@printf "Execute \"sudo apt install build-essential gcc-i386-linux-gnueabi nasm qemu-system gdb\""
	@printf "Or it's equivalent for your system"

#------------------
# Запуск отладчика
#------------------

GDB_FLAGS = \
	--eval-command="set architecture i386" \
	--eval-command="file $(EXECUTABLE)" \
	--eval-command="b main" \
	--eval-command="run" \
	--eval-command="tui e" \
	--eval-command="la regs"

gdb: $(EXECUTABLE)
	$(GDB) $(GDB_FLAGS)

#----------------------
# Запуск дизассемблера
#----------------------

disassemble:
	objdump -M intel --disassemble=assembled_example_function --visualize-jumps=color $(EXECUTABLE)

.PHONY: all clean qemu gdb

# Подключаем тестовую инфраструктуру.
PROGRAM=64bit_arithmetics
PROGRAM_NAME=30_64bit_arithmetics
TESTS = 00

include ../../homework/common.mk