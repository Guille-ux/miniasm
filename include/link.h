#ifndef MINIASM_LINK_H
#define MINIASM_LINK_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define SYM_TABLE_GROW_FACTOR 2

typedef enum {
	SYM_END=0,
	SYM_LABEL=1,
} SymbolType;

typedef struct {
	size_t id;
	size_t direction;
	bool ready;
	char *name;
} RelocationSymbol;

#define MAX_SYMBOL_NAME 17

typedef struct {
	size_t id;
	size_t value;
	SymbolType type;
	bool present;
	char name[17];
} __attribute__((packed)) Symbol;

typedef struct {
	Symbol sym_table[];
} __attribute__((packed)) SymTable;

typedef struct {
	size_t count;
	size_t cap;
	Symbol sym_table[];
} __attribute__((packed)) LinkerTable;

void mapRel2Sym(RelocationSymbol *rel, LinkerTable *table);

#endif
