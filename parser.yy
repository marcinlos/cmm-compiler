%{
#include <stdio.h>
#include "ast.h"

void print_error(const char*);
void yyerror(const char *);
int yylex(void);

#define  YYERROR_VERBOSE


// Korzeń drzewa AST
static node* program_root;

%}

%union {
    value_type type;
    char* string;
    int integer;
    char character;
    node* node;
    fun_def* fun_def;
    decl* decl;
    decl_list* decl_list;
    decltor* decltor;
    decltor_list* decltor_list;
    stmt_list* stmt_list;
    stmt* stmt;    
    expr* expr;
    expr_list* expr_list;
}


// Keywordy
%token <string> ID STRING
%token <integer> NUM
%token <character> CHAR
%token <type> CHAR_K INT_K VOID_K 
%token IF_K ELSE_K FOR_K WHILE_K EXTERN_K RETURN_K

// Operatory
%token OP_EQ OP_NEQ OP_LTE OP_GTE OP_AND OP_OR

%type <type> type
%type <node> program
%type <fun_def> function
%type <decl> declaration param_type fun_sig var_declaration
%type <decl_list> param_types var_declaration_list
%type <decltor> declarator function_declarator
%type <decltor_list> declarator_list function_declarators
%type <stmt_list> stmt_list
%type <stmt> stmt opt_assg assg
%type <expr_list> expr_list nonempty_expr_list
%type <expr> expr opt_expr 
%type <expr> logical_or_expr logical_and_expr
%type <expr> equality_expr relational_expr 
%type <expr> additive_expr multiplicative_expr 
%type <expr> unary_expr postfix_expr primary_expr

%destructor { free($$);                 } <string>
%destructor { free_nodes($$);           } <node>
%destructor { free_function($$);        } <fun_def>
%destructor { free_decl($$);            } <decl>
%destructor { free_decl_list($$);       } <decl_list>
%destructor { free_decltor($$);         } <decltor>
%destructor { free_decltor_list($$);    } <decltor_list>
%destructor { free_stmt($$);            } <stmt>
%destructor { free_stmt_list($$);       } <stmt_list>
%destructor { free_expr($$);            } <expr>
%destructor { free_expr_list($$);       } <expr_list>

%expect 1
%locations


%%

full_program : program          { program_root = $1; }

program
    :                           { $$ = NULL; }
    | program function          { $$ = append_fun_node($1, $2); }
    | program declaration ';'   { $$ = append_decl_node($1, $2); }
    ;

declaration
    : type declarator_list                  { $$ = make_decl($1, 0, $2); }
    | type function_declarators             { $$ = make_decl($1, 0, $2); }
    | EXTERN_K type function_declarators    { $$ = make_decl($2, 1, $3); }
    | VOID_K function_declarators           { $$ = make_decl($1, 0, $2); }
    | EXTERN_K VOID_K function_declarators  { $$ = make_decl($2, 1, $3); }
    ;

declarator_list
    : declarator                        { $$ = make_decltor_singleton($1); }
    | declarator_list ',' declarator    { $$ = append_decltor($1, $3); }
    ;

declarator
    : ID                    { $$ = make_decltor_id($1); }
    | ID '[' NUM ']'        { $$ = make_decltor_array($1, $3); }
    ;

type
    : CHAR_K
    | INT_K
    ;

function_declarators
    : function_declarator                           { $$ = make_decltor_singleton($1); }
    | function_declarators ',' function_declarator  { $$ = append_decltor($1, $3); }
    ;

function_declarator
    : ID '(' param_types ')'        { $$ = make_decltor_fun($1, $3); }
    | ID '(' error ')'              { yyerror("Invalid argument list"); $$ = NULL; }
    ;

param_types
    : VOID_K                        { $$ = NULL; }
    | param_type                    { $$ = make_decl_singleton($1); }
    | param_types ',' param_type    { $$ = append_decl($1, $3); }
    ;

param_type
    : type ID           { $$ = make_singular_decl($1, make_decltor_id($2)); }
    | type ID '[' ']'   { $$ = make_singular_decl($1, make_decltor_array($2, 0)); }
    ;


function
    : fun_sig '{' var_declaration_list stmt_list '}' { $$ = make_function($1, $3, $4); }
    | error   { yyerror("Invalid function declaration"); $$ = NULL; }
    ;
    
fun_sig
    : type function_declarator      { $$ = make_singular_decl($1, $2); }
    | VOID_K function_declarator    { $$ = make_singular_decl($1, $2); }
    ;

var_declaration_list
    :                                           { $$ = NULL; }
    | var_declaration_list var_declaration ';'  { $$ = append_decl($1, $2); }
    ;

var_declaration
    : type declarator_list                      { $$ = make_decl($1, 0, $2); }
    ;

expr_list
    :                       { $$ = NULL; }
    | nonempty_expr_list
    ;

nonempty_expr_list
    : expr                              { $$ = make_expr_singleton($1); }
    | nonempty_expr_list ',' expr       { $$ = append_expr($1, $3); }
    ;
    
stmt_list
    :                       { $$ = NULL; }
    | stmt_list stmt        { $$ = append_stmt($1, $2); }
    ;
    
opt_assg
    :                       { $$ = NULL; }
    | assg
    ;
    
opt_expr
    :                       { $$ = NULL; }
    | expr
    ;

stmt
    : IF_K '(' expr ')' stmt                                { $$ = make_if($3, $5, NULL); }
    | IF_K '(' expr ')' stmt ELSE_K stmt                    { $$ = make_if($3, $5, $7); }
    | WHILE_K '(' expr ')' stmt                             { $$ = make_while($3, $5); }
    | FOR_K '(' opt_assg ';' opt_expr ';' opt_assg ')' stmt { $$ = make_for($3, $5, $7, $9); }
    | RETURN_K opt_expr ';'                                 { $$ = make_return($2); }
    | assg ';'                                              
    | '{' stmt_list '}'                                     { $$ = make_compound($2); }
    | ';'                                                   { $$ = NULL; }
    | expr ';'                                              { $$ = make_expr_stmt($1); }
    | error ';'     { yyerror("Statement error"); $$ = NULL; }
    ;

assg
    : ID '=' expr               { $$ = make_assign($1, NULL, $3); }
    | ID '[' expr ']' '=' expr  { $$ = make_assign($1, $3, $6); }
    ;

expr
    : logical_or_expr
    ;

primary_expr
    : ID                    { $$ = make_id($1); }
    | NUM                   { $$ = make_int($1); }
    | CHAR                  { $$ = make_char($1); }
    | STRING                { $$ = make_string($1); }
    | '(' expr ')'          { $$ = $2; }
    ;

postfix_expr
    : ID '(' expr_list ')'  { $$ = make_call($1, $3); }
    | ID '(' error ')'      { yyerror("Error in function arguments"); $$ = NULL; }
    | primary_expr
    ;

unary_expr
    : '-' unary_expr        { $$ = make_unop(EXP_UNMINUS, $2); }
    | '!' unary_expr        { $$ = make_unop(EXP_NEG, $2) ;}
    | postfix_expr
    ;

multiplicative_expr
    : multiplicative_expr '*' unary_expr    { $$ = make_binop(EXP_TIMES, $1, $3); }
    | multiplicative_expr '/' unary_expr    { $$ = make_binop(EXP_DIV, $1, $3); }
    | unary_expr
    ;

additive_expr
    : additive_expr '+' multiplicative_expr { $$ = make_binop(EXP_PLUS, $1, $3); }
    | additive_expr '-' multiplicative_expr { $$ = make_binop(EXP_PLUS, $1, $3); } 
    | multiplicative_expr
    ;

relational_expr
    : relational_expr '<' additive_expr     { $$ = make_binop(EXP_LT, $1, $3); }
    | relational_expr '>' additive_expr     { $$ = make_binop(EXP_GT, $1, $3); }
    | relational_expr OP_LTE additive_expr  { $$ = make_binop(EXP_LTE, $1, $3); }
    | relational_expr OP_GTE additive_expr  { $$ = make_binop(EXP_GTE, $1, $3); }
    | additive_expr
    ;

equality_expr
    : equality_expr OP_EQ relational_expr   { $$ = make_binop(EXP_EQ, $1, $3); }
    | equality_expr OP_NEQ relational_expr  { $$ = make_binop(EXP_NEQ, $1, $3); }
    | relational_expr
    ;

logical_and_expr
    : logical_and_expr OP_AND equality_expr { $$ = make_binop(EXP_OR, $1, $3); }
    | equality_expr
    ;


logical_or_expr
    : logical_or_expr OP_OR logical_and_expr{ $$ = make_binop(EXP_AND, $1, $3); }
    | logical_and_expr
    ;


%%

void yyerror(const char* msg) 
{
    print_error(msg);
}

node* parse_stdin() 
{
    yyparse();
    return program_root;
}

