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
	table[table->count].id = rel->id;
	table[table->count].value = rel->direction;
	table[table->count].type = SYM_LABEL;
	memcpy(table[table->count].name, rel->name, MAX_SYMBOL_NAME);
	table[table->count].name[MAX_SYMBOL_NAME] = '\0';
	table[table->count++].present = rel->ready;
}
