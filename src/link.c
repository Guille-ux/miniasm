#include "../include/link.h"
#include "../include/compat.h"

void mapRel2Sym(RelocationSymbol *rel, LinkerTable *table) {
	if (table == NULL) return;
	if (table->count >= table->cap) {
		LinkerTable *new_table=(LinkerTable*)malloc(sizeof(size_t)*2+sizeof(Symbol)*table->cap*SYM_TABLE_GROW_FACTOR);
		memcpy(new_table, table, table->count);
		free(table);
		table = new_table;
	}
	table->sym_table[table->count].id = rel->id;
	table->sym_table[table->count].value = rel->direction;
	table->sym_table[table->count].type = SYM_LABEL;
	memcpy(table->sym_table[table->count].name, rel->name, MAX_SYMBOL_NAME);
	table->sym_table[table->count].name[MAX_SYMBOL_NAME] = '\0';
	table->sym_table[table->count++].present = rel->ready;
}


void newRelocationRequest(ByteStream *stream, size_t pos, size_t size, const char *tag, size_t len) {
	Symbol newSym;
	newSym.present = false;
	newSym.size = size;
	newSym.value = pos;
	newSym.type = SYM_RELOCATION_REQUEST;
	memcpy(newSym.name, tag, (MAX_SYMBOL_NAME > len) ? len : MAX_SYMBOL_NAME);
	newSym.name[(len < MAX_SYMBOL_NAME)? len : MAX_SYMBOL_NAME] = '\0';
	
	streamAppendBytes(stream, (uint8_t*)&newSym, sizeof(Symbol));
}
