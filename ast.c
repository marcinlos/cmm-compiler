/*
 * ast.c
 *
 *  Created on: Dec 16, 2012
 *      Author: los
 */

#include "ast.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>



static void clean_node(node* n)
{
    n->declaration = NULL;
    n->function = NULL;
    n->next = NULL;
}

static node* make_empty_node(void)
{
    node* n = malloc(sizeof(node));
    clean_node(n);
    return n;
}

node* make_fun_node(fun_def* function)
{
    node* n = make_empty_node();
    n->type = NODE_FUN;
    n->function = function;
    return n;
}

node* make_decl_node(decl* declaration)
{
    node* n = make_empty_node();
    n->type = NODE_DECL;
    n->declaration = declaration;
    return n;
}

static node* append_node(node* old, node* n)
{
    node* head = old, *prev = NULL;
    while (head != NULL)
    {
        prev = head;
        head = head->next;
    }
    if (prev != NULL)
        prev->next = n;
    else
        old = n;
    return old;
}

node* append_fun_node(node* list, fun_def* function)
{
    node* n = make_fun_node(function);
    return append_node(list, n);
}

node* append_decl_node(node* list, decl* declaration)
{
    node* n = make_decl_node(declaration);
    return append_node(list, n);
}

void free_nodes(node* root)
{
    while (root != NULL)
    {
        node* tmp = root;
        root = root->next;
        if (tmp->type == NODE_DECL)
            free_decl(tmp->declaration);
        else if (tmp->type == NODE_FUN)
            free_function(tmp->function);
        clean_node(tmp);
        free(tmp);
    }
}


fun_def* make_function(decl* signature, decl_list* decls, stmt_list* stmts)
{
    fun_def* fun = malloc(sizeof(fun_def));
    fun->signature = signature;
    fun->declarations = decls;
    fun->statements = stmts;
    return fun;
}

void free_function(fun_def* fun)
{
    if (fun != NULL)
    {
        free_decl(fun->signature);
        free_decl_list(fun->declarations);
        free_stmt_list(fun->statements);
        free(fun);
    }
}


decl* make_decl(value_type type, int ext, decltor_list* list)
{
    decl* d = malloc(sizeof(decl));
    d->declarators = list;
    d->is_extern = ext;
    d->type = type;
    return d;
}

decl* make_singular_decl(value_type type, decltor* decl)
{
    decltor_list* one = make_decltor_singleton(decl);
    return make_decl(type, 0, one);
}

void free_decl(decl* d)
{
    if (d != NULL)
    {
        free_decltor_list(d->declarators);
        free(d);
    }
}



decl_list* make_decl_singleton(decl* d)
{
    decl_list* list = malloc(sizeof(decl_list));
    list->declaration = d;
    list->next = NULL;
    return list;
}

decl_list* append_decl(decl_list* list, decl* d)
{
    decl_list* elem = make_decl_singleton(d);
    decl_list* head = list, *prev = NULL;
    while (head)
    {
        prev = head;
        head = head->next;
    }
    if (prev != NULL)
        prev->next = elem;
    else
        list = elem;
    return list;
}

void free_decl_list(decl_list* list)
{
    while (list != NULL)
    {
        decl_list* tmp = list;
        list = list->next;
        free_decl(tmp->declaration);
        free(tmp);
    }
}


static decltor* make_empty_decltor(void)
{
    decltor* d = malloc(sizeof(decltor));
    d->elements = NULL;
    d->name = NULL;
    return d;
}

decltor* make_decltor_id(char* name)
{
    decltor* d = make_empty_decltor();
    d->type = DECLTOR_SIMPLE;
    d->name = name;
    return d;
}

decltor* make_decltor_array(char* name, int size)
{
    decltor* d = make_empty_decltor();
    d->type = DECLTOR_ARRAY;
    d->name = name;
    d->elements = make_int(size);
    return d;
}

decltor* make_decltor_fun(char* name, decl_list* params)
{
    decltor* d = make_empty_decltor();
    d->type = DECLTOR_FUN;
    d->name = name;
    d->params = params;
    return d;
}

void free_decltor(decltor* d)
{
    if (d != NULL)
    {
        if (d->type == DECLTOR_ARRAY)
            free_expr(d->elements);
        else if (d->type == DECLTOR_FUN)
            free_decl_list(d->params);
        free(d->name);
        free(d);
    }
}


decltor_list* make_decltor_singleton(decltor* d)
{
    decltor_list* list = malloc(sizeof(decltor_list));
    list->declarator = d;
    list->next = NULL;
    return list;
}

decltor_list* append_decltor(decltor_list* list, decltor* d)
{
    decltor_list* elem = make_decltor_singleton(d);
    decltor_list* head = list, *prev = NULL;
    while (head != NULL)
    {
        prev = head;
        head = head->next;
    }
    if (prev != NULL)
        prev->next = elem;
    else
        list = elem;
    return list;
}

void free_decltor_list(decltor_list* list)
{
    while (list != NULL)
    {
        decltor_list* tmp = list;
        list = list->next;
        free_decltor(tmp->declarator);
        free(tmp);
    }
}


static stmt* make_empty_stmt(void)
{
    stmt* s = calloc(1, sizeof(stmt));
    return s;
}

stmt* make_expr_stmt(expr* e)
{
    stmt* s = make_empty_stmt();
    s->type = STM_EXP;
    s->expression = e;
    return s;
}

stmt* make_assign(char* name, expr* index, expr* value)
{
    stmt* s = make_empty_stmt();
    s->type = STM_ASSIGN;
    s->name = name;
    s->index = index;
    s->expression = value;
    return s;
}

stmt* make_return(expr* e)
{
    stmt* s = make_empty_stmt();
    s->type = STM_RETURN;
    s->expression = e;
    return s;
}

stmt* make_while(expr* cond, stmt* body)
{
    stmt* s = make_empty_stmt();
    s->type = STM_WHILE;
    s->condition = cond;
    s->body = body;
    return s;
}

stmt* make_for(stmt* init, expr* cond, stmt* update, stmt* body)
{
    stmt* s = make_empty_stmt();
    s->type = STM_FOR;
    s->init = init;
    s->condition = cond;
    s->update = update;
    s->body = body;
    return s;
}

stmt* make_if(expr* cond, stmt* body, stmt* else_body)
{
    stmt* s = make_empty_stmt();
    s->type = STM_IF;
    s->condition = cond;
    s->body = body;
    s->else_body = else_body;
    return s;
}

stmt* make_compound(stmt_list* list)
{
    stmt* s = make_empty_stmt();
    s->type = STM_COMPOUND;
    s->list = list;
    return s;
}

void free_stmt(stmt* s)
{
    if (s != NULL)
    {
        if (s->type >= STM_EXP && s->type < STM_COMPOUND)
        {
            if (s->name != NULL)
                free(s->name);
            free_expr(s->expression);
            free_expr(s->index);
        }
        if (s->type < STM_EXP)
        {
            free_stmt(s->body);
            free_stmt(s->init);
            free_expr(s->condition);
            free_stmt(s->update);
            free_stmt(s->else_body);
        }
        if (s->type == STM_COMPOUND)
            free_stmt_list(s->list);
        free(s);
    }
}


stmt_list* make_stmt_singleton(stmt* s)
{
    stmt_list* list = malloc(sizeof(stmt_list));
    list->statement = s;
    list->next = NULL;
    return list;
}

stmt_list* append_stmt(stmt_list* list, stmt* s)
{
    stmt_list* elem = make_stmt_singleton(s);
    stmt_list* head = list, *prev = NULL;
    while (head != NULL)
    {
        prev = head;
        head = head->next;
    }
    if (prev != NULL)
        prev->next = elem;
    else
        list = elem;
    return list;
}

void free_stmt_list(stmt_list* list)
{
    while (list != NULL)
    {
        stmt_list* tmp = list;
        list = list->next;
        free_stmt(tmp->statement);
        free(tmp);
    }
}

expr* make_empty_expr(void)
{
    expr* e = calloc(1, sizeof(expr));
    return e;
}

expr* make_binop(expr_type type, expr* left, expr* right)
{
    expr* e = make_empty_expr();
    e->type = type;
    e->left = left;
    e->right = right;
    return e;
}

expr* make_unop(expr_type type, expr* operand)
{
    expr* e = make_empty_expr();
    e->type = type;
    e->expression = operand;
    return e;
}

expr* make_string(char* string)
{
    expr* e = make_empty_expr();
    e->type = EXP_STRING;
    e->string = string;
    return e;
}

expr* make_int(int value)
{
    expr* e = make_empty_expr();
    e->type = EXP_INT;
    e->integer = value;
    return e;
}

expr* make_char(char value)
{
    expr* e = make_empty_expr();
    e->type = EXP_CHAR;
    e->character = value;
    return e;
}

expr* make_id(char* id)
{
    expr* e = make_empty_expr();
    e->type = EXP_ID;
    e->id = id;
    return e;
}

expr* make_call(char* fun, expr_list* args)
{
    expr* e = make_empty_expr();
    e->type = EXP_CALL;
    e->id = fun;
    e->args = args;
    return e;
}

expr* make_index(expr* base, expr* index)
{
    expr* e = make_empty_expr();
    e->type = EXP_INDEX;
    e->expression = base;
    e->index = index;
    return e;
}

void free_expr(expr* e)
{
    if (e != NULL)
    {
        if (e->type == EXP_ID)
        {
            if (e->id != NULL)
                free(e->id);
        }
        if (e->type == EXP_STRING)
        {
            if (e->string != NULL)
                free(e->string);
        }
        if (e->type >= EXP_PLUS && e->type < EXP_UNMINUS)
        {
            free_expr(e->left);
            free_expr(e->right);
        }
        if (e->type >= EXP_UNMINUS && e->type < EXP_INDEX)
        {
            free_expr(e->expression);
        }
        if (e->type == EXP_INDEX)
        {
            free_expr(e->expression);
            free_expr(e->index);
        }
        if (e->type == EXP_CALL)
        {
            if (e->id != NULL)
                free(e->id);
            free_expr_list(e->args);
        }
        free(e);
    }
}


expr_list* make_expr_singleton(expr* e)
{
    expr_list* list = malloc(sizeof(list));
    list->expression = e;
    list->next = NULL;
    return list;
}

expr_list* append_expr(expr_list* list, expr* e)
{
    expr_list* elem = make_expr_singleton(e);
    expr_list* head = list, *prev = NULL;
    while (head != NULL)
    {
        prev = head;
        head = head->next;
    }
    if (prev != NULL)
        prev->next = elem;
    else
        list = elem;
    return elem;
}

void free_expr_list(expr_list* list)
{
    while (list != NULL)
    {
        expr_list* tmp = list;
        list = list->next;
        free_expr(tmp->expression);
        free(tmp);
    }
}
