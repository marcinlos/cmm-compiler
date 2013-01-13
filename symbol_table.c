#include <stdlib.h>
#include <assert.h>
#include "symbol_table.h"


scope_info* make_scope(scope_info* parent)
{
    scope_info* scope = malloc(sizeof(scope_info));
    scope->parent = parent;
    scope->symbols = NULL;
    return scope;
}

void free_scope_info(scope_info* scope)
{
    assert(scope && "free_scope_info: NULL argument");
    free_symbol_info(scope->symbols);
}

void free_symbol_info(symbol_info* sym)
{
    assert(sym && "free_symbol_info: NULL argument");
    free(sym->name);
    if (sym->next)
        free_symbol_info(sym->next);
}


void clean_symbol_table(symbol_table* table)
{
    table->top_level = NULL;
}

