
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
BOOT = boot
KERNEL = kernel
DRIVER = driver
INCLUDE = include

LINKER_SCRIPT = script/linker.ld

CFLAGS = -Wall -Wextra -ffreestanding -m32 -fno-pie -fno-stack-protector -O1 -Wno-implicit-fallthrough

default:
	mkdir -p bin
	$(NASM) $(BOOT)/boot.asm -f bin -o $(BIN)/boot.bin
	$(NASM) $(BOOT)/entry.asm -f elf -o $(BIN)/entry.o
	$(CC) $(CFLAGS) -c $(KERNEL)/kernel.c -o $(BIN)/kernel.o -I $(INCLUDE)
	$(CC) $(CFLAGS) -c $(DRIVER)/video.c -o $(BIN)/video.o -I $(INCLUDE)
	$(CC) $(CFLAGS) -c $(DRIVER)/keyboard.c -o $(BIN)/keyboard.o -I $(INCLUDE)
	$(LD) -m elf_i386 -T $(LINKER_SCRIPT) -o $(BIN)/kernel.bin $(BIN)/entry.o $(BIN)/kernel.o $(BIN)/video.o $(BIN)/keyboard.o --oformat binary
	(cat $(BIN)/boot.bin ; cat $(BIN)/kernel.bin) > $(BIN)/$(OUT)

run:
	qemu-system-x86_64 $(BIN)/$(OUT)

clean:
	rm -drf $(BIN)
