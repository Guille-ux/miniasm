# Makefile de MiniAsm

CC = gcc
ASM = nasm
AR = ar
ARFLAGS = rcs
CFLAGS = -c # Las banderas para GCC 

SRC = src
BUILD = build

$(BUILD)/miniasm.o: $(SRC)/miniasm.c
	$(CC) $(CFLAGS) -o $(BUILD)/miniasm.o $(SRC)/miniasm.c

$(BUILD)/stream.o: $(SRC)/stream.c
	$(CC) $(CFLAGS) -o $(BUILD)/stream.o $(SRC)/stream.c

$(BUILD)/utils.o: $(SRC)/utils.c
	$(CC) $(CFLAGS) -o $(BUILD)/utils.o $(SRC)/utils.c

.PHONY: clean all

clean:
	rm -rf $(BUILD)/*

all:
