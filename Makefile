
#
# Makefile
#
# Antoine LANDRIEUX
# BORIUM <https://github.com/AntoineLandrieux/BORIUM>
#
# MIT License
#

OUT = borium.bin

LD = ld
CC = gcc
NASM = nasm

BIN = bin
STD = std
CORE = core
BOOT = boot
KERNEL = kernel
DRIVER = driver
INCLUDE = include

LINKER_SCRIPT = script/linker.ld

CFLAGS = -Wall -Wextra -ffreestanding -m32 -fno-pie -fno-stack-protector -O1 -Wno-implicit-fallthrough

default:
	mkdir -p bin
	#
	$(NASM) $(BOOT)/boot.asm -f bin -o $(BIN)/boot.bin
	$(NASM) $(BOOT)/entry.asm -f elf -o $(BIN)/entry.o
	#
	$(CC) $(CFLAGS) -c $(KERNEL)/kernel.c -o $(BIN)/kernel.o -I $(INCLUDE)
	#
	$(CC) $(CFLAGS) -c $(DRIVER)/video.c -o $(BIN)/dvideo.o -I $(INCLUDE)
	$(CC) $(CFLAGS) -c $(DRIVER)/keyboard.c -o $(BIN)/dkeyboard.o -I $(INCLUDE)
	#
	$(CC) $(CFLAGS) -c $(STD)/stdlib.c -o $(BIN)/stdlib.o -I $(INCLUDE)
	#
	$(CC) $(CFLAGS) -c $(CORE)/Error.c -o $(BIN)/Error.o -I $(INCLUDE)
	$(CC) $(CFLAGS) -c $(CORE)/Math.c -o $(BIN)/Math.o -I $(INCLUDE)
	$(CC) $(CFLAGS) -c $(CORE)/Memory.c -o $(BIN)/Memory.o -I $(INCLUDE)
	$(CC) $(CFLAGS) -c $(CORE)/Parser.c -o $(BIN)/Parser.o -I $(INCLUDE)
	$(CC) $(CFLAGS) -c $(CORE)/Runtime.c -o $(BIN)/Runtime.o -I $(INCLUDE)
	$(CC) $(CFLAGS) -c $(CORE)/Tokenizer.c -o $(BIN)/Tokenizer.o -I $(INCLUDE)
	#
	$(LD) -m elf_i386 -T $(LINKER_SCRIPT) -o $(BIN)/kernel.bin $(BIN)/entry.o $(BIN)/kernel.o $(BIN)/dvideo.o $(BIN)/dkeyboard.o $(BIN)/stdlib.o $(BIN)/Error.o $(BIN)/Math.o $(BIN)/Memory.o $(BIN)/Parser.o $(BIN)/Runtime.o $(BIN)/Tokenizer.o --oformat binary
	(cat $(BIN)/boot.bin ; cat $(BIN)/kernel.bin) > $(BIN)/$(OUT)

run:
	qemu-system-x86_64 $(BIN)/$(OUT)

clean:
	rm -drf $(BIN)
