#include "mips.h"


MIPS_list* MIPS_make_binop(ir_item* item)
{

}

MIPS_list* MIPS_make_unop(ir_item* item)
{

}

MIPS_list* MIPS_make_copy(ir_item* item)
{

}

MIPS_list* MIPS_make_goto(ir_item* item)
{
    return MIPS_single_code(MIPS_mk1(
            MIPS_b,
            MIPS_mksymbol(item->x.symbol_name)));
}

MIPS_list* MIPS_make_branch(ir_item* item)
{

}

MIPS_list* MIPS_make_lidx(ir_item* item)
{

}

MIPS_list* MIPS_make_ridx(ir_item* item)
{

}

MIPS_list* MIPS_make_addr(ir_item* item)
{

}

MIPS_list* MIPS_make_lderef(ir_item* item)
{

}

MIPS_list* MIPS_make_rderef(ir_item* item)
{

}

MIPS_list* MIPS_make_enter(ir_item* item)
{
    MIPS_list* node = MIPS_empty_list();
    node->instr = MIPS_mk3(
            MIPS_sw,
            MIPS_mkreg(R_FP),
            MIPS_mkreg(R_SP),
            MIPS_mkoffset(-4));

    MIPS_append_item(node, MIPS_mk3(
            MIPS_sw,
            MIPS_mkreg(R_RA),
            MIPS_mkreg(R_SP),
            MIPS_mkoffset(-8)));

    MIPS_append_item(node, MIPS_mk3(
            MIPS_la,
            MIPS_mkreg(R_FP),
            MIPS_mkreg(R_SP),
            MIPS_mkoffset(0)));
    int n = 3;

    MIPS_append_item(node, MIPS_mk3(
            MIPS_la,
            MIPS_mkreg(R_SP),
            MIPS_mkreg(R_SP),
            MIPS_mkoffset(-n)));
    return node;
}

MIPS_list* MIPS_make_leave(ir_item* item)
{

}

MIPS_list* MIPS_make_param(ir_item* item)
{

}

MIPS_list* MIPS_make_call(ir_item* item)
{

}

MIPS_list* MIPS_make_ret(ir_item* item)
{

}

MIPS_list* MIPS_make_ret_val(ir_item* item)
{

}

MIPS_list* MIPS_make_retrieve(ir_item* item)
{

}
