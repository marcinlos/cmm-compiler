#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_

#include "types.h"
#include "codegen.h"

#define TABLE_SIZE 5


typedef enum
{
    SYM_FUN,
    SYM_VAR
} symbol_type;


typedef struct scope_info
{
    struct symbol_info* symbols;
    struct scope_info* parent;
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
        };
    };
    struct symbol_info* next;
} symbol_info;


typedef struct symbol_table
{
    scope_info* top_level;
} symbol_table;


scope_info* make_scope(scope_info* parent);
void free_scope_info(scope_info* scope);

void free_symbol_info(symbol_info* sym);

void clean_symbol_table(symbol_table* table);


#endif // SYMBOL_TABLE_H_
