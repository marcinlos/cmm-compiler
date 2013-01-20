#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ir.h"
#include "util.h"

#define BUF_SIZE 128


address ir_mkempty(void)
{
    address addr;
    addr.type = ADDR_EMPTY;
    return addr;
}

int ir_is_empty(address* addr)
{
    return addr->type == ADDR_EMPTY;
}

address ir_mkint(int n)
{
    address addr;
    addr.type = ADDR_IMM_INT;
    addr.integer = n;
    return addr;
}

address ir_mkchar(char c)
{
    address addr;
    addr.type = ADDR_IMM_CHAR;
    addr.character = c;
    return addr;
}

address ir_mkstring(const char* string)
{
    address addr;
    addr.type = ADDR_IMM_STRING;
    addr.string = strdup(string);
    return addr;
}

address ir_mksymbol_string(const char* name)
{
    address addr;
    addr.type = ADDR_SYMBOL_NAME;
    addr.symbol_name = strdup(name);
    return addr;
}

address ir_mksymbol(symbol_info* symbol)
{
    address addr;
    addr.type = ADDR_SYMBOL;
    addr.symbol = symbol;
    return addr;
}

void ir_free_address(address* addr)
{
    assert(addr && "ir_free_address: NULL argument");
    switch (addr->type)
    {
    case ADDR_IMM_STRING: free(addr->string); break;
    case ADDR_SYMBOL_NAME: free(addr->symbol_name); break;
    }
}


ir_item ir_mkitem(void)
{
    ir_item item;
    address empty = ir_mkempty();
    item.y = item.x = item.z = empty;
    item.type = IR_EMPTY;
    return item;
}

ir_item ir_mklabel(const char* label, int offset)
{
    ir_item item = ir_mkitem();
    item.type = IR_LABEL;
    item.label = strdup(label);
    item.offset = offset;
    return item;
}

ir_item ir_mkdata_raw(const char* name, int bytes)
{
    ir_item item = ir_mkitem();
    item.type = IR_DATA;
    item.data_type = DATA_RAW;
    item.name = strdup(name);
    item.bytes = bytes;
    return item;
}

ir_item ir_mkdata_string(const char* name, const char* string)
{
    ir_item item = ir_mkitem();
    item.type = IR_DATA;
    item.data_type = DATA_STRING;
    item.name = strdup(name);
    item.text = strdup(string);
    return item;
}


ir_item ir_mkbinop(address x, address y, address z, ir_binop op)
{
    ir_item item = ir_mkitem();
    item.type = IR_BINOP;
    item.x = x;
    item.y = y;
    item.z = z;
    item.binop = op;
    return item;
}

ir_item ir_mkunop(address x, address y, ir_unop op)
{
    ir_item item = ir_mkitem();
    item.type = IR_UNOP;
    item.x = x;
    item.y = y;
    item.unop = op;
    return item;
}

ir_item ir_mkcopy(address x, address y)
{
    ir_item item = ir_mkitem();
    item.type = IR_COPY;
    item.x = x;
    item.y = y;
    return item;
}

ir_item ir_mkgoto(const char* label)
{
    ir_item item = ir_mkitem();
    item.type = IR_GOTO;
    item.x = ir_mksymbol_string(label);
    return item;
}

ir_item ir_mkbranch(address x, address y, ir_relop op, const char* label)
{
    ir_item item = ir_mkitem();
    item.type = IR_BRANCH;
    item.x = x;
    item.y = y;
    item.relop = op;
    item.z = ir_mksymbol_string(label);
    return item;
}

ir_item ir_mklidx(address x, address y, address z)
{
    ir_item item = ir_mkitem();
    item.type = IR_LIDX;
    item.x = x;
    item.y = y;
    item.z = z;
    return item;
}

ir_item ir_mkridx(address x, address y, address z)
{
    ir_item item = ir_mkitem();
    item.type = IR_RIDX;
    item.x = x;
    item.y = y;
    item.z = z;
    return item;
}

ir_item ir_mkaddr(address x, address y)
{
    ir_item item = ir_mkitem();
    item.type = IR_ADDR;
    item.x = x;
    item.y = y;
    return item;
}

ir_item ir_mklderef(address x, address y)
{
    ir_item item = ir_mkitem();
    item.type = IR_LDEREF;
    item.x = x;
    item.y = y;
    return item;
}

ir_item ir_mkrderef(address x, address y)
{
    ir_item item = ir_mkitem();
    item.type = IR_RDEREF;
    item.x = x;
    item.y = y;
    return item;
}

ir_item ir_mkenter(const char* function)
{
    ir_item item = ir_mkitem();
    item.type = IR_ENTER;
    item.x = ir_mksymbol_string(function);
    return item;
}

ir_item ir_mkleave(const char* function)
{
    ir_item item = ir_mkitem();
    item.type = IR_LEAVE;
    item.x = ir_mksymbol_string(function);
    return item;
}

ir_item ir_mkparam(address x)
{
    ir_item item = ir_mkitem();
    item.type = IR_PARAM;
    item.x = x;
    return item;
}

ir_item ir_mkcall(const char* function)
{
    ir_item item = ir_mkitem();
    item.type = IR_CALL;
    item.x = ir_mksymbol_string(function);
    return item;
}

ir_item ir_mkcall_ptr(address f)
{
    ir_item item = ir_mkitem();
    item.type = IR_CALL;
    item.x = f;
    return item;
}

ir_item ir_mkret(void)
{
    ir_item item = ir_mkitem();
    item.type = IR_RET;
    return item;
}

ir_item ir_mkret_val(address x)
{
    ir_item item = ir_mkitem();
    item.type = IR_RET_VAL;
    item.x = x;
    return item;
}

ir_item ir_mkretrieve(address x)
{
    ir_item item = ir_mkitem();
    item.type = IR_RETRIEVE;
    item.x = x;
    return item;
}


void ir_free_item(ir_item* item)
{
    assert(item && "ir_free_item: NULL argument");
    switch (item->type)
    {
    case IR_LABEL: free(item->label); break;
    case IR_DATA: free(item->name); break;
    default:
        ir_free_address(&item->x);
        ir_free_address(&item->y);
        ir_free_address(&item->z);
        break;
    }
}


int ir_print_address(char* buffer, address addr)
{
    int n;
    switch (addr.type)
    {
    case ADDR_SYMBOL:
        strcpy(buffer, addr.symbol->name);
        n = strlen(addr.symbol->name);
        break;

    case ADDR_SYMBOL_NAME:
        strcpy(buffer, addr.symbol_name);
        n = strlen(addr.symbol_name);
        break;

    case ADDR_IMM_INT:
        n = sprintf(buffer, "%d", addr.integer);
        break;

    case ADDR_IMM_CHAR:
        n = sprintf(buffer, "%d", (int) addr.character);
        break;

    case ADDR_IMM_STRING:
    {
        char* escaped = escape_alloc(addr.string);
        strcpy(buffer, escaped);
        n = strlen(escaped);
        free(escaped);
        break;
    }
    }
    return n;
}

static const char* print_binop(ir_binop binop)
{
    switch (binop)
    {
    case IR_PLUS:   return "+";
    case IR_MINUS:  return "-";
    case IR_TIMES:  return "*";
    case IR_DIV:    return "/";
    case IR_AND:    return "&";
    case IR_OR:     return "|";
    default:
        assert(0 && "print_binop: invalid operator");
        return NULL;
    }
}

static const char* print_unop(ir_unop unop)
{
    switch (unop)
    {
    case IR_NEG:    return "-";
    default:
        assert(0 && "print_unop: invalid operator");
        return NULL;
    }
}

static const char* print_relop(ir_relop relop)
{
    switch (relop)
    {
    case IR_EQ:     return "=";
    case IR_NEQ:    return "=/=";
    case IR_GT:     return ">";
    case IR_GTE:    return ">=";
    case IR_LT:     return "<";
    case IR_LTE:    return "<=";
    }
}

static char* print_data(char* buffer, ir_item* item)
{
    char data[BUF_SIZE];
    switch (item->data_type)
    {
    case DATA_RAW:
        sprintf(data, "%s .space %d", item->name, item->bytes);
        strcat(buffer, data);
        break;

    case DATA_STRING:
    {
        char* escaped = escape_alloc(item->text);
        sprintf(data, "%s .ascii \"%s\"", item->name, escaped);
        free(escaped);
        strcat(buffer, data);
        break;
    }
    }
    return buffer;
}

static char* print_binop_code(char* buffer, ir_item* item)
{
    char* p = buffer;
    p += ir_print_address(p, item->x);
    p += sprintf(p, " := ");
    p += ir_print_address(p, item->y);
    p += sprintf(p, " %s ", print_binop(item->binop));
    p += ir_print_address(p, item->z);
    return buffer;
}

static char* print_unop_code(char* buffer, ir_item* item)
{
    char* p = buffer;
    p += ir_print_address(p, item->x);
    p += sprintf(p, " := ");
    p += sprintf(p, "%s", print_unop(item->unop));
    p += ir_print_address(p, item->y);
    return buffer;
}

static char* print_copy_code(char* buffer, ir_item* item)
{
    char* p = buffer;
    p += ir_print_address(p, item->x);
    p += sprintf(p, " := ");
    p += ir_print_address(p, item->y);
    return buffer;
}

static char* print_goto_code(char* buffer, ir_item* item)
{
    char* p = buffer;
    p += sprintf(p, "jump ");
    p += ir_print_address(p, item->x);
    return buffer;
}

static char* print_branch_code(char* buffer, ir_item* item)
{
    char* p = buffer;
    p += sprintf(p, "if ");
    p += ir_print_address(p, item->x);
    p += sprintf(p, " %s ", print_relop(item->relop));
    p += ir_print_address(p, item->y);
    p += sprintf(p, " goto ");
    p += ir_print_address(p, item->z);
    return buffer;
}

static char* print_label_code(char* buffer, ir_item* item)
{
    char* p = buffer;
    p += sprintf(p, "%s:", item->label);
    return buffer;
}

static char* print_lidx_code(char* buffer, ir_item* item)
{
    char* p = buffer;
    p += ir_print_address(p, item->x);
    p += sprintf(p, "[");
    p += ir_print_address(p, item->y);
    p += sprintf(p, "] := ");
    p += ir_print_address(p, item->z);
    return buffer;
}

static char* print_ridx_code(char* buffer, ir_item* item)
{
    char* p = buffer;
    p += ir_print_address(p, item->x);
    p += sprintf(p, ":= ");
    p += ir_print_address(p, item->y);
    p += sprintf(p, "[");
    p += ir_print_address(p, item->z);
    p += sprintf(p, "]");
    return buffer;
}

static char* print_addr_code(char* buffer, ir_item* item)
{
    char* p = buffer;
    p += ir_print_address(p, item->x);
    p += sprintf(p, " := &");
    p += ir_print_address(p, item->y);
    return buffer;
}

static char* print_lderef_code(char* buffer, ir_item* item)
{
    char* p = buffer;
    p += sprintf(buffer, "*");
    p += ir_print_address(p, item->x);
    p += sprintf(p, " := ");
    p += ir_print_address(p, item->y);
    return buffer;
}

static char* print_rderef_code(char* buffer, ir_item* item)
{
    char* p = buffer;
    p += ir_print_address(p, item->x);
    p += sprintf(p, " := *");
    p += ir_print_address(p, item->y);
    return buffer;
}

static char* print_enter_code(char* buffer, ir_item* item)
{
    char* p = buffer;
    p += sprintf(p, "enter ");
    p += ir_print_address(p, item->x);
    return buffer;
}

static char* print_leave_code(char* buffer, ir_item* item)
{
    char* p = buffer;
    p += sprintf(p, "leave ");
    p += ir_print_address(p, item->x);
    return buffer;
}

static char* print_param_code(char* buffer, ir_item* item)
{
    char* p = buffer;
    p += sprintf(p, "param ");
    p += ir_print_address(p, item->x);
    return buffer;
}

static char* print_call_code(char* buffer, ir_item* item)
{
    char* p = buffer;
    p += sprintf(p, "leave ");
    p += ir_print_address(p, item->x);
    return buffer;
}

static char* print_retval_code(char* buffer, ir_item* item)
{
    char* p = buffer;
    p += sprintf(p, "ret ");
    p += ir_print_address(p, item->x);
    return buffer;
}

static char* print_retrieve_code(char* buffer, ir_item* item)
{
    char* p = buffer;
    p += sprintf(p, "retrieve ");
    p += ir_print_address(p, item->x);
    return buffer;
}

char* ir_pretty_print(char* buffer, ir_item item)
{
    switch (item.type)
    {
    case IR_DATA:       print_data(buffer, &item);           break;
    case IR_BINOP:      print_binop_code(buffer, &item);     break;
    case IR_UNOP:       print_unop_code(buffer, &item);      break;
    case IR_COPY:       print_copy_code(buffer, &item);      break;
    case IR_GOTO:       print_goto_code(buffer, &item);      break;
    case IR_BRANCH:     print_branch_code(buffer, &item);    break;
    case IR_LABEL:      print_label_code(buffer, &item);     break;
    case IR_LIDX:       print_lidx_code(buffer, &item);      break;
    case IR_RIDX:       print_ridx_code(buffer, &item);      break;
    case IR_ADDR:       print_addr_code(buffer, &item);      break;
    case IR_LDEREF:     print_lderef_code(buffer, &item);    break;
    case IR_RDEREF:     print_rderef_code(buffer, &item);    break;
    case IR_ENTER:      print_enter_code(buffer, &item);     break;
    case IR_LEAVE:      print_leave_code(buffer, &item);     break;
    case IR_PARAM:      print_param_code(buffer, &item);     break;
    case IR_CALL:       print_call_code(buffer, &item);      break;
    case IR_RET:        return "ret";                        break;
    case IR_RET_VAL:    print_retval_code(buffer, &item);    break;
    case IR_RETRIEVE:   print_retrieve_code(buffer, &item);  break;
    }
    return buffer;
}
