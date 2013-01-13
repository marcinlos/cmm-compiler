#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"

typedef struct
{
    ir_code* code;
    ir_list* last;
    scope_info* scope;
} context;


static void process_decl(node* node, context* ctx)
{
    printf("Decl: %s\n", node->declaration->declarators->declarator->name);
}


static void process_fun(node* node, context* ctx)
{
    printf("Fun: %s\n", node->function->signature->declarators->declarator->name);
}

/*
 * Processes top level entity - function/global variable declaration,
 * or function definition.
 */
static void process_toplevel(node* node, context* ctx)
{
    switch (node->type)
    {
    case NODE_DECL: process_decl(node, ctx); break;
    case NODE_FUN:  process_fun(node, ctx);  break;
    }
}

/*
 * Produces clean, initialized ir_code structure.
 */
static ir_code* make_code_struct(void)
{
    ir_code* code = malloc(sizeof(ir_code));
    code->codes = NULL;
    clean_symbol_table(&code->table);
    return code;
}



ir_code* build_ir(node* ast)
{
    ir_code* code = make_code_struct();
    scope_info* top = make_scope(NULL);
    code->table.top_level = top;
    context ctx = { code, NULL, top };
    while (ast != NULL)
    {
        process_toplevel(ast, &ctx);
        ast = ast->next;
    }
    return code;
}
