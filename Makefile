# Makefile de MiniAsm

CC = gcc
ASM = nasm
AR = ar
ARFLAGS = rcs
CFLAGS = # Las banderas para GCC 

miniasm.o: src/miniasm.c
	$(CC) $(CFLAGS) -o miniasm.o src/miniasm.c

stream.o: src/stream.c
	$(CC) $(CFLAGS) -o stream.o src/stream.c
