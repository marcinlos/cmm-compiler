#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "symbol_table.h"

static symbol_info* empty_symbol(void)
{
    symbol_info* sym = malloc(sizeof(symbol_info));
    sym->symbol_type = SYM_NONE;
    sym->name = NULL;
    sym->next = NULL;
    sym->scope = NULL;
    return sym;
}

static symbol_info* append_symbol(symbol_info* list, symbol_info* node)
{
    symbol_info* head = list, *prev = NULL;
    while (head != NULL)
    {
        prev = head;
        head = head->next;
    }
    if (prev != NULL)
        prev->next = node;
    else
        list = node;
    return list;
}


symbol_info* sym_mkvar(const char* name, type type, codegen_info gen_info,
        scope_info* scope)
{
    symbol_info* sym = empty_symbol();
    sym->symbol_type = SYM_VAR;
    sym->name = strdup(name);
    sym->var_type = type;
    sym->codegen = gen_info;
    sym->scope = scope;
    return sym;
}

symbol_info* sym_mkfunction(const char* name, type ret, scope_info* scope)
{
    symbol_info* sym = empty_symbol();
    sym->symbol_type = SYM_FUN;
    sym->name = strdup(name);
    sym->ret_type = ret;
    sym->scope = scope;
    printf("przed mkscope name = %s\n", sym->name);
    sym->params = sym_mkscope(scope);
    printf("przed add name = %s\n", sym->name);
    //sym_add_child_scope(scope, sym->params);
    return sym;
}

void sym_add_param(symbol_info* fun, symbol_info* param)
{
    assert(fun && "sym_add_param: NULL function");
    assert(fun && "sym_add_param: NULL param");
    sym_add_symbol(fun->params, param);
}

void free_symbol_info(symbol_info* sym)
{
    assert(sym && "free_symbol_info: NULL argument");
    symbol_info* next = sym->next;
    free(sym->name);
    free(sym);
    if (next)
    {
        free_symbol_info(next);
    }
}

scope_info* sym_mkscope(scope_info* parent)
{
    scope_info* scope = malloc(sizeof(scope_info));
    scope->parent = parent;
    scope->next = NULL;
    scope->children = NULL;
    scope->symbols = NULL;
    return scope;
}


void sym_add_symbol(scope_info* scope, symbol_info* symbol)
{
    assert(scope && "sym_add_symbol: NULL scope");
    assert(symbol && "sym_add_symbol: NULL symbol");
    scope->symbols = append_symbol(scope->symbols, symbol);
}

void sym_add_child_scope(scope_info* parent, scope_info* child)
{
    assert(parent && "sym_add_child_scope: NULL parent");
    assert(child && "sym_add_child_scope: NULL child");

    if (parent->children == NULL)
        parent->children = child;
    else
    {
        scope_info* i = parent->children;
        while (i ->next != NULL)
            i = i->next;
        i->next = child;
    }
}

void free_scope_info(scope_info* scope)
{
    assert(scope && "free_scope_info: NULL argument");
    if (scope->symbols != NULL)
        free_symbol_info(scope->symbols);
    if (scope->children != NULL)
        free_scope_info(scope->children);
    if (scope->next != NULL)
        free_scope_info(scope->next);
    free(scope);
}



void clean_symbol_table(symbol_table* table)
{
    table->top_level = NULL;
}

void free_symbol_table(symbol_table* table)
{
    free_scope_info(table->top_level);
    free(table);
}


static int print_scope_tree(char* buffer, scope_info* scope)
{
    assert(scope && "print_scope_tree: NULL scope");
    char* p = buffer;
    p += sym_print_scope(buffer, scope);
    scope_info* child = scope->children;
    while (child != NULL)
    {
        p += print_scope_tree(p, child);
        child = child->next;
    }
    return p - buffer;
}

int sym_print_table(char* buffer, symbol_table* table)
{
    assert(table && "sym_print_table: NULL table");
    return print_scope_tree(buffer, table->top_level);
}

int sym_print_scope(char* buffer, scope_info* scope)
{
    assert(scope && "sym_print_scope: NULL scope");
    char* p = buffer;
    p += sprintf(p, "scope: %p (parent %p)\n", scope, scope->parent);
    symbol_info* symbol = scope->symbols;
    while (symbol != NULL)
    {
        p += sym_print_symbol(p, symbol);
        p += sprintf(p, "\n");
        symbol = symbol->next;
    }
    return p - buffer;
}

int sym_print_symbol(char* buffer, symbol_info* info)
{
    assert(info && "sym_print_symbol: NULL info");
    char* p = buffer;
    p += sprintf(p, "%s:\t", info->name);
    char type_buffer[256];
    switch (info->symbol_type)
    {
    case SYM_FUN:
        type_print(type_buffer, info->ret_type);
        p += sprintf(p, "() -> %s", type_buffer);
        break;

    case SYM_VAR:
        type_print(type_buffer, info->var_type);
        p += sprintf(p, "%s", type_buffer);
        break;
    }
    return p - buffer;
}

