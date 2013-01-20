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

static void ctx_push_new_scope(context* ctx)
{
    scope_info* parent = ctx->scope;
    scope_info* scope = sym_mkscope(parent);
    sym_add_child_scope(parent, scope);
    ctx->scope = scope;
}

static void ctx_push_scope(context* ctx, scope_info* scope)
{
    sym_add_child_scope(ctx->scope, scope);
    ctx->scope = scope;
}

static void ctx_pop_scope(context* ctx)
{
    ctx->scope = ctx->scope->parent;
}

static void ctx_add_symbol(context* ctx, symbol_info* symbol)
{
    sym_add_symbol(ctx->scope, symbol);
}


static void append_item(context* ctx, ir_item item)
{
    ctx->last = ir_append_item(ctx->last, item);
    if (ctx->code->codes == NULL)
        ctx->code->codes = ctx->last;
}

static void process_decl(node* node, context* ctx)
{
    //printf("Decl: %s\n", node->declaration->declarators->declarator->name);
}


static void process_fun(node* node, context* ctx)
{
    decl* sig = node->function->signature;
    char* name = sig->declarators->declarator->name;
    type t= type_from_ast(sig->type);
    symbol_info* fun = sym_mkfunction(name, t, ctx->scope);
    ctx_add_symbol(ctx, fun);
    // Formal parameters scope
    ctx_push_scope(ctx, fun->params);
    decl_list* param_list = sig->declarators->declarator->params;

    while (param_list != NULL)
    {
        decl* d = param_list->declaration;
        decltor* param = d->declarators->declarator;
        value_type t = d->type;
        codegen_info cg = { ST_UNKNOWN };
        type tp = type_from_ast(t);
        symbol_info* par = sym_mkvar(param->name, tp, cg, fun->params);
        ctx_add_symbol(ctx, par);
        param_list = param_list->next;
    }
    // Body scope
    ctx_push_new_scope(ctx);

    ctx_pop_scope(ctx);
    ctx_pop_scope(ctx);
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
    scope_info* top = sym_mkscope(NULL);
    code->table.top_level = top;
    context ctx = { code, NULL, top };
    while (ast != NULL)
    {
        process_toplevel(ast, &ctx);
        ast = ast->next;
    }
    return code;
}
