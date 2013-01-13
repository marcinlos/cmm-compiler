/*
 * ast.h
 *
 *  Created on: Dec 16, 2012
 *      Author: los
 */

#ifndef AST_H_
#define AST_H_


typedef enum
{
    TYPE_INT,
    TYPE_CHAR,
    TYPE_VOID,
} value_type;


typedef enum
{
    NODE_DECL,
    NODE_FUN
} node_type;


typedef enum
{
    DECLTOR_FUN,
    DECLTOR_ARRAY,
    DECLTOR_SIMPLE
} decltor_type;


typedef enum
{
    STM_IF,
    STM_WHILE,
    STM_FOR,
    STM_EXP = 100,
    STM_RETURN,
    STM_ASSIGN,
    STM_COMPOUND = 200
} stmt_type;


typedef enum
{
    EXP_ID,
    EXP_INT,
    EXP_CHAR,
    EXP_STRING,
    EXP_PLUS = 100,
    EXP_MINUS,
    EXP_TIMES,
    EXP_DIV,
    EXP_EQ,
    EXP_NEQ,
    EXP_LTE,
    EXP_GTE,
    EXP_LT,
    EXP_GT,
    EXP_AND,
    EXP_OR,
    EXP_UNMINUS = 150,
    EXP_NEG,
    EXP_INDEX = 200,
    EXP_CALL
} expr_type;


typedef struct node
{
    node_type type;
    union
    {
        struct fun_def* function;
        struct decl* declaration;
    };
    struct node* next;
} node;


typedef struct fun_def
{
    struct decl* signature;
    struct decl_list* declarations;
    struct stmt_list* statements;
} fun_def;


typedef struct decl_list
{
    struct decl* declaration;
    struct decl_list* next;
} decl_list;

typedef struct decl
{
    value_type type;
    int is_extern;
    struct decltor_list* declarators;
} decl;


typedef struct decltor_list
{
    struct decltor* declarator;
    struct decltor_list* next;
} decltor_list;


typedef struct decltor
{
    decltor_type type;
    char* name;
    union
    {
        struct decl_list* params;
        struct expr* elements;
    };
} decltor;


typedef struct stmt_list
{
    struct stmt* statement;
    struct stmt_list* next;
} stmt_list;


typedef struct stmt
{
    stmt_type type;
    union
    {
        struct
        {
            char* name;
            struct expr* index;
            struct expr* expression;
        };
        struct
        {
            struct stmt* init;
            struct expr* condition;
            struct stmt* update;
            struct stmt* body;
            struct stmt* else_body;
        };
        stmt_list* list;
    };
} stmt;


typedef struct expr_list
{
    struct expr* expression;
    struct expr_list* next;
} expr_list;


typedef struct expr
{
    expr_type type;
    union
    {
        union
        {
            struct
            {
                struct expr* left;
                struct expr* right;
            };
            struct
            {
                char* id;
                struct expr_list* args;
            };
            struct
            {
                struct expr* expression;
                struct expr* index;
            };
        };
        union
        {
            char* string;
            int integer;
            char character;
        };
    };

} expr;


node* make_fun_node(fun_def* function);
node* make_decl_node(decl* declaration);
node* append_fun_node(node* list, fun_def* function);
node* append_decl_node(node* list, decl* declaration);
void free_nodes(node* root);

fun_def* make_function(decl* signature, decl_list* decls, stmt_list* stmts);
void free_function(fun_def* fun);


decl* make_decl(value_type type, int ext, decltor_list* list);
decl* make_singular_decl(value_type type, decltor* decl);
void free_decl(decl* d);

decl_list* make_decl_singleton(decl* d);
decl_list* append_decl(decl_list* list, decl* d);
void free_decl_list(decl_list* list);


decltor* make_decltor_id(char* name);
decltor* make_decltor_array(char* name, int size);
decltor* make_decltor_fun(char* name, decl_list* params);
void free_decltor(decltor* d);

decltor_list* make_decltor_singleton(decltor* d);
decltor_list* append_decltor(decltor_list* list, decltor* d);
void free_decltor_list(decltor_list* list);


stmt* make_expr_stmt(expr* e);
stmt* make_assign(char* name, expr* index, expr* value);
stmt* make_return(expr* e);
stmt* make_while(expr* cond, stmt* body);
stmt* make_for(stmt* init, expr* cond, stmt* update, stmt* body);
stmt* make_if(expr* cond, stmt* body, stmt* else_body);
stmt* make_compound(stmt_list* list);
void free_stmt(stmt* s);

stmt_list* make_stmt_singleton(stmt* s);
stmt_list* append_stmt(stmt_list* list, stmt* s);
void free_stmt_list(stmt_list* list);


expr* make_binop(expr_type type, expr* left, expr* right);
expr* make_unop(expr_type type, expr* operand);
expr* make_string(char* string);
expr* make_int(int value);
expr* make_char(char value);
expr* make_id(char* id);
expr* make_call(char* fun, expr_list* args);
expr* make_index(expr* base, expr* index);
void free_expr(expr* e);

expr_list* make_expr_singleton(expr* e);
expr_list* append_expr(expr_list* list, expr* e);
void free_expr_list(expr_list* list);



#endif /* AST_H_ */
