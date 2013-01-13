#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parser.h"
#include "ir.h"
#include "util.h"

#include "mips.h"


#define BUF_SIZE 128



static line_list* out_data(ir_item* item);
static line_list* out_label(ir_item* item);
static MIPS_list* MIPS_translate(ir_item* item);

line_list* ir_output(ir_item item)
{
    switch (item.type)
    {
    case IR_EMPTY: return NULL;
    case IR_DATA: return out_data(&item);
    case IR_LABEL: return out_label(&item);
    }

    MIPS_list* list = MIPS_translate(&item), *mp = list;
    line_list* lines = NULL, *end = lines;
    char buffer[BUF_SIZE];
    while (mp != NULL)
    {
        MIPS_write_instr(buffer, mp->instr);
        end = append_line(end, buffer);
        if (lines == NULL)
            lines = end;
        mp = mp->next;
    }
    MIPS_free_list(list);
    return lines;

}

static MIPS_list* MIPS_translate(ir_item* item)
{
    switch (item->type)
    {
    case IR_BINOP:      return MIPS_make_binop(item);
    case IR_UNOP:       return MIPS_make_unop(item);
    case IR_COPY:       return MIPS_make_copy(item);
    case IR_GOTO:       return MIPS_make_goto(item);
    case IR_BRANCH:     return MIPS_make_branch(item);
    case IR_LIDX:       return MIPS_make_lidx(item);
    case IR_RIDX:       return MIPS_make_ridx(item);
    case IR_ADDR:       return MIPS_make_addr(item);
    case IR_LDEREF:     return MIPS_make_lderef(item);
    case IR_RDEREF:     return MIPS_make_rderef(item);
    case IR_ENTER:      return MIPS_make_enter(item);
    case IR_LEAVE:      return MIPS_make_leave(item);
    case IR_PARAM:      return MIPS_make_param(item);
    case IR_CALL:       return MIPS_make_call(item);
    case IR_RET:        return MIPS_make_ret(item);
    case IR_RET_VAL:    return MIPS_make_ret_val(item);
    case IR_RETRIEVE:   return MIPS_make_retrieve(item);
    }
}

/*
 * Produces asm lines for data declaration
 */
static line_list* out_data(ir_item* item)
{
    char buffer[BUF_SIZE];
    char data[BUF_SIZE];
    sprintf(buffer, "%s:\t\t", item->name);
    switch (item->data_type)
    {
    case DATA_RAW:
        sprintf(data, ".space %d", item->bytes);
        strcat(buffer, data);
        break;

    case DATA_STRING:
    {
        char* escaped = escape_alloc(item->text);
        sprintf(data, ".ascii \"%s\"", escaped);
        free(escaped);
        strcat(buffer, data);
        break;
    }
    }
    return single_line(buffer);
}


static line_list* out_label(ir_item* item)
{
    return single_line(item->label);
}


