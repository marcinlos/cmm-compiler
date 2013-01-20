/*
 * Header file containing structures and functions used to create and
 * manipulate intermediate representation (IR) of the program.
 */
#ifndef IR_H_
#define IR_H_

#include "symbol_table.h"


typedef enum
{
    IR_EMPTY,
    IR_DATA,
    IR_BINOP,
    IR_UNOP,
    IR_COPY,
    IR_GOTO,        // goto x
    IR_BRANCH,      // if x relop y goto z
    IR_LABEL,
    IR_LIDX,        // x[y] := z
    IR_RIDX,        // x := y[z]
    IR_ADDR,        // x := &y
    IR_LDEREF,      // *x := y
    IR_RDEREF,      // x := *y

    IR_ENTER,       // enter procedure x (push info)
    IR_LEAVE,       // leave procedure x (pop info)
    IR_PARAM,       // push param
    IR_CALL,        // call x, y
    IR_RET,         // return
    IR_RET_VAL,     // return x
    IR_RETRIEVE     // x = returned value
} ir_type;


typedef enum
{
    ADDR_EMPTY,
    ADDR_SYMBOL,
    ADDR_SYMBOL_NAME,
    ADDR_IMM_INT,
    ADDR_IMM_CHAR,
    ADDR_IMM_STRING
} addr_type;


typedef struct address
{
    addr_type type;
    union
    {
        int integer;
        char character;
        char* string;
        symbol_info* symbol;
        char* symbol_name;
    };
} address;

typedef enum
{
    IR_PLUS,
    IR_MINUS,
    IR_TIMES,
    IR_DIV,
    IR_AND,
    IR_OR
} ir_binop;


typedef enum
{
    IR_NEG
} ir_unop;


typedef enum
{
    IR_EQ,
    IR_NEQ,
    IR_GT,
    IR_GTE,
    IR_LT,
    IR_LTE
} ir_relop;

typedef enum
{
    DATA_RAW,
    DATA_STRING
} ir_data;


typedef struct
{
    ir_type type;
    union
    {
        struct
        {
            address x, y, z;
            union
            {
                ir_binop binop;
                ir_unop unop;
                ir_relop relop;
            };
        };
        struct
        {
            char* name;
            ir_data data_type;
            union
            {
                int bytes;
                char* text;
            };
        };
        struct
        {
            char* label;
            int offset;
        };
    };
} ir_item;

address ir_mkempty(void);
int ir_is_empty(address* addr);

address ir_mkint(int n);
address ir_mkchar(char c);
address ir_mkstring(const char* string);
address ir_mksymbol_string(const char* name);
address ir_mksymbol(symbol_info* symbol);

void ir_free_address(address* addr);

ir_item ir_mkitem(void);

ir_item ir_mklabel(const char* label, int offset);
ir_item ir_mkdata_raw(const char* name, int bytes);
ir_item ir_mkdata_string(const char* name, const char* string);

ir_item ir_mkbinop(address x, address y, address z, ir_binop op);
ir_item ir_mkunop(address x, address y, ir_unop op);
ir_item ir_mkcopy(address x, address y);
ir_item ir_mkgoto(const char* label);
ir_item ir_mkbranch(address x, address y, ir_relop op, const char* label);
ir_item ir_mklidx(address x, address y, address z);
ir_item ir_mkridx(address x, address y, address z);
ir_item ir_mkaddr(address x, address y);
ir_item ir_mklderef(address x, address y);
ir_item ir_mkrderef(address x, address y);

ir_item ir_mkenter(const char* function);
ir_item ir_mkleave(const char* function);
ir_item ir_mkparam(address x);
ir_item ir_mkcall(const char* function);
ir_item ir_mkcall_ptr(address f);
ir_item ir_mkret(void);
ir_item ir_mkret_val(address x);
ir_item ir_mkretrieve(address x);

void ir_free_item(ir_item* item);


typedef struct ir_list
{
    ir_item item;
    struct ir_list* next;
} ir_list;

ir_list* ir_empty_list(void);
ir_list* ir_single_code(ir_item item);
ir_list* ir_append_item(ir_list* last, ir_item item);
void ir_free_list(ir_list* codes);


char* ir_pretty_print(char* buffer, ir_item item);
int ir_print_address(char* buffer, address addr);


#endif // IR_H_
