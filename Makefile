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

$(BUILD)/libminiasm.a: all
	$(AR) $(ARFLAGS) $(BUILD)/libminiasm.o $(BUILD)/*.o

$(BUILD)/test.o: $(SRC)/test.c
	$(CC) $(CFLAGS) -o $(BUILD)/test.o $(SRC)/test.c

.PHONY: clean all lib test run_test

clean:
	rm -rf $(BUILD)/*

lib: $(BUILD)/libminiasm.a

test: $(BUILD)/test.o all
	$(CC) -o $(BUILD)/test $(BUILD)/*.o

run_test: test
	@echo "Running Test..."
	./$(BUILD)/test

all: $(BUILD)/data_asm.o $(BUILD)/directives.o $(BUILD)/utils.o $(BUILD)/miniasm.o  $(BUILD)/stream.o
