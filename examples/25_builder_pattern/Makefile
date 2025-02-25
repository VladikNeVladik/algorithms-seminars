# Copyright 2025 Vladislav Aleinik

#-------------------------
# Флаги сборки и линковки
#-------------------------

CXX = g++

CXXFLAGS =     \
	-std=c++20 \
	-Wall      \
	-Wextra    \
	-Werror

LDFLAGS =

ifeq ($(DEBUG),1)
	CXXFLAGS += -g
else
	CXXFLAGS  += -flto -DNDEBUG
	LXXDFLAGS += -flto
endif

#-------
# Цвета
#-------

BRED    = \033[1;31m
BGREEN  = \033[1;32m
BYELLOW = \033[1;33m
GREEN   = \033[1;35m
BCYAN   = \033[1;36m
RESET   = \033[0m

#-------
# Файлы
#-------

INCLUDES = \
	include/builder.hpp

CXXFLAGS += -I $(abspath include)

SOURCES = \
	src/builder.cpp \
	src/test.cpp

OBJECTS = $(SOURCES:src/%.cpp=build/%.o)

EXECUTABLE = build/test

#----------------
# Процесс сборки
#----------------

default: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@printf "$(BYELLOW)Linking executable $(BCYAN)$@$(RESET)\n"
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

build/%.o: src/%.cpp $(INCLUDES)
	@printf "$(BYELLOW)Building object file $(BCYAN)$@$(RESET)\n"
	@mkdir -p build
	$(CXX) -c $< $(CXXFLAGS) -o $@

#----------------------
# Вспомогательные цели
#----------------------

run: $(EXECUTABLE)
	@printf "$(BYELLOW)Running executable$(RESET)\n"
	@mkdir -p res
	@./$(EXECUTABLE)

clean:
	@printf "$(BYELLOW)Cleaning build and resource directories$(RESET)\n"
	rm -rf res
	rm -rf build

.PHONY: run clean default
