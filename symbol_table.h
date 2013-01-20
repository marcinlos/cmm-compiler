#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_

#include "types.h"
#include "codegen.h"

#define TABLE_SIZE 5


typedef enum
{
    SYM_NONE,
    SYM_FUN,
    SYM_VAR
} symbol_type;


typedef struct scope_info
{
    struct symbol_info* symbols;
    struct scope_info* parent;
    struct scope_info* next;
    struct scope_info* children;
} scope_info;


typedef struct symbol_info
{
    symbol_type symbol_type;
    char* name;
    scope_info* scope;
    union
    {
        struct
        {
            type var_type;
            codegen_info codegen;
        };
        struct
        {
            type ret_type;
            scope_info* params;
        };
    };
    struct symbol_info* next;
} symbol_info;


typedef struct symbol_table
{
    scope_info* top_level;
} symbol_table;



symbol_info* sym_mkvar(const char* name, type type, codegen_info gen_info, scope_info* scope);
symbol_info* sym_mkfunction(const char* name, type ret, scope_info* scope);
void sym_add_param(symbol_info* fun, symbol_info* param);
void free_symbol_info(symbol_info* sym);

scope_info* sym_mkscope(scope_info* parent);
void sym_add_symbol(scope_info* scope, symbol_info* symbol);
void sym_add_child_scope(scope_info* parent, scope_info* child);
void free_scope_info(scope_info* scope);


void clean_symbol_table(symbol_table* table);
void free_symbol_table(symbol_table* table);

int sym_print_table(char* buffer, symbol_table* table);
int sym_print_scope(char* buffer, scope_info* scope);
int sym_print_symbol(char* buffer, symbol_info* info);

#endif // SYMBOL_TABLE_H_
