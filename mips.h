#ifndef MIPS_H_
#define MIPS_H_

#include "parser.h"


typedef enum
{
    MIPS_add,
    MIPS_sub,
    MIPS_addi,
    MIPS_mult,
    MIPS_div,
    MIPS_lw,
    MIPS_lb,
    MIPS_sw,
    MIPS_sb,
    MIPS_mfhi,
    MIPS_mflo,
    MIPS_and,
    MIPS_andi,
    MIPS_or,
    MIPS_ori,
    MIPS_slt,
    MIPS_slti,
    MIPS_bgt,
    MIPS_bge,
    MIPS_blt,
    MIPS_ble,
    MIPS_sll,
    MIPS_srl,
    MIPS_beq,
    MIPS_bne,
    MIPS_jal,
    MIPS_jr,
    MIPS_b,
    MIPS_la,
    MIPS_li
} MIPS_opcode;


typedef enum
{
    MIPS_OPERAND_NONE,
    MIPS_REG,
    MIPS_IMM_32,
    MIPS_IMM_8,
    MIPS_OFFSET,
    MIPS_SYMBOL,
} MIPS_operand_type;

#define R_FP 30
#define R_SP 29
#define R_RA 31
#define R_ZERO 0


typedef struct
{
    MIPS_operand_type type;
    union
    {
        int reg;
        int imm32;
        char imm8;
        int offset;
        char* symbol;
    };

} MIPS_operand;


typedef struct
{
    MIPS_opcode opcode;
    MIPS_operand x, y, z;
} MIPS_instruction;


const char* MIPS_op_name(MIPS_opcode opcode);

MIPS_operand MIPS_mkimm32(int n);
MIPS_operand MIPS_mkimm8(char c);
MIPS_operand MIPS_mkreg(int reg);
MIPS_operand MIPS_mkoffset(int offset);
MIPS_operand MIPS_mksymbol(const char* symbol);

void MIPS_free_operand(MIPS_operand* operand);

MIPS_instruction MIPS_mk1(MIPS_opcode op, MIPS_operand x);
MIPS_instruction MIPS_mk2(MIPS_opcode op, MIPS_operand x, MIPS_operand y);
MIPS_instruction MIPS_mk3(MIPS_opcode op, MIPS_operand x, MIPS_operand y,
        MIPS_operand z);

int MIPS_write_operand(char* buffer, MIPS_operand operand);
char* MIPS_write_instr(char* buffer, MIPS_instruction instr);

void MIPS_free_instr(MIPS_instruction* instruction);


typedef struct MIPS_list
{
    MIPS_instruction instr;
    struct MIPS_list* next;
} MIPS_list;

MIPS_list* MIPS_empty_list(void);
MIPS_list* MIPS_single_code(MIPS_instruction item);
MIPS_list* MIPS_append_item(MIPS_list* last, MIPS_instruction item);
void MIPS_free_list(MIPS_list* codes);


MIPS_list* MIPS_make_binop(ir_item* item);
MIPS_list* MIPS_make_unop(ir_item* item);
MIPS_list* MIPS_make_copy(ir_item* item);
MIPS_list* MIPS_make_goto(ir_item* item);
MIPS_list* MIPS_make_branch(ir_item* item);
MIPS_list* MIPS_make_lidx(ir_item* item);
MIPS_list* MIPS_make_ridx(ir_item* item);
MIPS_list* MIPS_make_addr(ir_item* item);
MIPS_list* MIPS_make_lderef(ir_item* item);
MIPS_list* MIPS_make_rderef(ir_item* item);
MIPS_list* MIPS_make_enter(ir_item* item);
MIPS_list* MIPS_make_leave(ir_item* item);
MIPS_list* MIPS_make_param(ir_item* item);
MIPS_list* MIPS_make_call(ir_item* item);
MIPS_list* MIPS_make_ret(ir_item* item);
MIPS_list* MIPS_make_ret_val(ir_item* item);
MIPS_list* MIPS_make_retrieve(ir_item* item);

#endif // MIPS_H_
