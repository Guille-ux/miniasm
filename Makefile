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

$(BUILD)/data_asm.o: $(SRC)/data_asm.c
	$(CC) $(CFLAGS) -o $(BUILD)/data_asm.o $(SRC)/data_asm.c

$(BUILD)/directives.o: $(SRC)/directives.c
	$(CC) $(CFLAGS) -o $(BUILD)/directives.o $(SRC)/directives.c

.PHONY: clean all

clean:
	rm -rf $(BUILD)/*

all: $(BUILD)/data_asm.o $(BUILD)/directives.o $(BUILD)/utils.o $(BUILD)/miniasm.o  $(BUILD)/stream.o
