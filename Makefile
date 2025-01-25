
#
#  _____  _____  ___  ______ _____
# /  ___||  _  |/ _ \ | ___ \  ___|
# \ `--. | | | / /_\ \| |_/ / |__
#  `--. \| | | |  _  ||    /|  __|
# /\__/ /\ \_/ / | | || |\ \| |___
# \____/  \___/\_| |_/\_| \_\____/
#
# Antoine LANDRIEUX (MIT License) <Makefile>
# <https://github.com/AntoineLandrieux/SOARE/>
#

APP = soare.exe

CC = gcc
CPP = g++

BIN = bin
LIB = lib
SRC = src
CORE = core
INCLUDE = include

CFLAGS = -Wall -Wextra -Werror -Wpedantic -Wno-implicit-fallthrough

# DEBUG = -D __SOARE_DEBUG

default: $(BIN)/$(APP)

$(LIB)/libsoare.a: $(CORE_OBJS)

CORE_OBJS := $(patsubst $(CORE)/%.c, $(LIB)/%.o, $(wildcard $(CORE)/*.c))

$(LIB)/%.o: $(CORE)/%.c
	mkdir -p $(LIB)
	$(CC) -c $< -o $@ -I $(INCLUDE) $(CFLAGS) $(DEBUG)

$(BIN)/$(APP): $(CORE_OBJS) $(SRC)/Main.cpp
	mkdir -p $(BIN)
	ar rcs $(LIB)/libsoare.a $(CORE_OBJS)
	$(CPP) $(SRC)/Main.cpp -o $(BIN)/$(APP) -I $(INCLUDE) -L$(LIB) -lsoare $(CFLAGS) $(DEBUG)

run:
	$(BIN)/$(APP)

clean:
	rm -rf $(BIN) $(LIB)
