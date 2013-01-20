#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "parser.h"

typedef struct
{
    ir_code* code;
    ir_list* last;
    scope_info* scope;
    int tmp_counter;
    int label_counter;
} context;

static address ctx_mktmp(context* ctx)
{
    char buffer[32];
    sprintf(buffer, "t%d", ++ ctx->tmp_counter);
    return ir_mksymbol_string(buffer);
}

static ir_item ctx_mklabel(context* ctx)
{
    char buffer[32];
    sprintf(buffer, "__L%d__", ++ ctx->label_counter);
    return ir_mklabel(buffer, 0);
}

static address ctx_mkliteral(const char* literal, context* ctx)
{
    symbol_info* info = sym_add_literal(literal, &ctx->code->table);
    return ir_mksymbol(info);
}

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

static symbol_info* ctx_lookup(const char* name, context* ctx)
{
    return sym_find(name, ctx->scope);
}


static void append_item(ir_item item, context* ctx)
{
    ctx->last = ir_append_item(ctx->last, item);
    if (ctx->code->codes == NULL)
        ctx->code->codes = ctx->last;
}

static void ctx_put_label(const char* label, context* ctx)
{
    ir_item lab = ir_mklabel(label, 0);
    append_item(lab, ctx);
}

static void process_decl(node* node, context* ctx)
{
    //printf("Decl: %s\n", node->declaration->declarators->declarator->name);
}

//------------------------------------------------------------------------------
// Statement processing functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Expression processing functions
//------------------------------------------------------------------------------

static address process_expr_id(expr* exp, context* ctx)
{
    printf("expr_id (%s)\n", exp->id);
    symbol_info* info = ctx_lookup(exp->id, ctx);
    if (info == NULL)
    {
        fprintf(stderr, "[%s]: Error: unknown identifier `%s'\n",
                "???", exp->id);
        return ir_mkempty();
    }
    else
        return ir_mksymbol(info);
}

static address process_expr_string(expr* exp, context* ctx)
{
    printf("expr_string (%s)\n", exp->string);
    return ctx_mkliteral(exp->string, ctx);
}

static address process_expr_call(expr* exp, context* ctx)
{
    printf("expr_call (%s)\n", exp->id);
    return ir_mkint(3);
}




static address process_expr(expr* exp, context* ctx)
{
    switch (exp->type)
    {
    case EXP_ID: return process_expr_id(exp, ctx);
    case EXP_INT: return ir_mkint(exp->integer);
    case EXP_CHAR: return ir_mkchar(exp->character);
    case EXP_STRING: return process_expr_string(exp, ctx);
    case EXP_CALL: return process_expr_call(exp, ctx);
    }
}

static void process_return(expr* exp, context* ctx)
{
    printf("Return statement: %p\n", exp);
}

//------------------------------------------------------------------------------

static void process_statement(stmt* statement, context* ctx)
{
    switch (statement->type)
    {
    case STM_ASSIGN: break;
    case STM_COMPOUND: break;

    case STM_EXP:
        process_expr(statement->expression, ctx);
        break;

    case STM_FOR: break;
    case STM_IF: break;

    case STM_RETURN:
        process_return(statement->expression, ctx);
        break;

    case STM_WHILE: break;
    default:
        assert(0 && "process_statement: unknown statement type");
        break;
    }
}

static void process_body(stmt_list* stms, context* ctx)
{
    while (stms != NULL)
    {
        process_statement(stms->statement, ctx);
        stms = stms->next;
    }
}


static void process_fun(node* node, context* ctx)
{
    decl* sig = node->function->signature;
    const char* name = sig->declarators->declarator->name;
    ctx_put_label(name, ctx);
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

    process_body(node->function->statements, ctx);

    // Pop body scope
    ctx_pop_scope(ctx);
    // Pop function parameter scope
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

static void prepend_strings(ir_code* code)
{
    ir_list* strings = NULL, *last = strings;
    symbol_info* s = code->table.literals;
    while (s != NULL)
    {
        ir_item str = ir_mkdata_string(s->name, s->literal);
        last = ir_append_item(last, str);
        if (strings == NULL)
            strings = last;
        s = s->next;
    }
    if (last != NULL)
    {
        last->next = code->codes;
        code->codes = strings;
    }
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
    prepend_strings(code);
    return code;
}
