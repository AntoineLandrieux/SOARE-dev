
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
VERSION_MAJ = 1

CC = gcc
CPP = g++

BIN = bin
LIB = lib
SRC = src
CORE = core
INCLUDE = include

CFLAGS = -Wall -Wextra -Wpedantic -Werror

DEBUG = -D __SOARE_DEBUG

default: $(BIN)/$(APP)

$(LIB)/libsoare.a: $(CORE_OBJS)

CORE_OBJS := $(patsubst $(CORE)/%.c, $(LIB)/%.o, $(wildcard $(CORE)/*.c))

$(LIB):
	mkdir -p $(LIB)

$(LIB)/%.o: $(CORE)/%.c
	$(CC) -c $< -o $@ -I $(INCLUDE) $(CFLAGS) $(DEBUG)

$(BIN)/$(APP): $(LIB) $(CORE_OBJS) $(SRC)/Main.cpp
	mkdir -p $(BIN)
	ar rcs $(LIB)/libsoare$(VERSION_MAJ).a $(CORE_OBJS)
	$(CPP) $(SRC)/Main.cpp -o $(BIN)/$(APP) -I $(INCLUDE) -L$(LIB) -lsoare$(VERSION_MAJ) $(CFLAGS) $(DEBUG)
	rm $(CORE_OBJS)

run:
	$(BIN)/$(APP)

clean:
	rm -rf $(BIN) $(LIB)
