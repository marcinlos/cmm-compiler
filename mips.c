#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "mips.h"

const char* MIPS_op_name(MIPS_opcode opcode)
{
    switch (opcode)
    {
    case MIPS_add:   return "add";  break;
    case MIPS_sub:   return "sub";  break;
    case MIPS_addi:  return "addi"; break;
    case MIPS_mult:  return "mult"; break;
    case MIPS_div:   return "div";  break;
    case MIPS_lw:    return "lw";   break;
    case MIPS_lb:    return "lb";   break;
    case MIPS_sw:    return "sw";   break;
    case MIPS_sb:    return "sb";   break;
    case MIPS_mfhi:  return "mhfi"; break;
    case MIPS_mflo:  return "mflo"; break;
    case MIPS_and:   return "and";  break;
    case MIPS_andi:  return "andi"; break;
    case MIPS_or:    return "or";   break;
    case MIPS_ori:   return "ori";  break;
    case MIPS_slt:   return "slt";  break;
    case MIPS_slti:  return "slti"; break;
    case MIPS_bgt:   return "bgt";  break;
    case MIPS_bge:   return "bge";  break;
    case MIPS_blt:   return "blt";  break;
    case MIPS_ble:   return "ble";  break;
    case MIPS_sll:   return "sll";  break;
    case MIPS_srl:   return "srl";  break;
    case MIPS_beq:   return "beq";  break;
    case MIPS_bne:   return "bne";  break;
    case MIPS_jal:   return "jal";  break;
    case MIPS_jr:    return "jr";   break;
    case MIPS_b:     return "b";    break;
    case MIPS_la:    return "la";   break;
    case MIPS_li:    return "li";   break;
    }
    assert(0 && "MIPS_op_name: invalid opcode");
    return "INVALID";
}

static MIPS_operand empty_operand(void)
{
    MIPS_operand op;
    op.type = MIPS_OPERAND_NONE;
    return op;
}

MIPS_operand MIPS_mkimm32(int n)
{
    MIPS_operand op;
    op.type = MIPS_IMM_32;
    op.imm32 = n;
    return op;
}

MIPS_operand MIPS_mkimm8(char c)
{
    MIPS_operand op;
    op.type = MIPS_IMM_8;
    op.imm8 = c;
    return op;
}

MIPS_operand MIPS_mkreg(int reg)
{
    MIPS_operand op;
    op.type = MIPS_REG;
    op.reg = reg;
    return op;
}

MIPS_operand MIPS_mkoffset(int offset)
{
    MIPS_operand op;
    op.type = MIPS_OFFSET;
    op.offset = offset;
    return op;
}

MIPS_operand MIPS_mksymbol(const char* symbol)
{
    MIPS_operand op;
    op.type = MIPS_SYMBOL;
    op.symbol = strdup(symbol);
    return op;
}


void MIPS_free_operand(MIPS_operand* operand)
{
    assert(operand && "MIPS_free_operand: NULL argument");
    switch (operand->type)
    {
    case MIPS_SYMBOL: free(operand->symbol); break;
    }
}


MIPS_instruction MIPS_mk1(MIPS_opcode op, MIPS_operand x)
{
    MIPS_instruction instr;
    instr.opcode = op;
    instr.x = x;
    instr.y = instr.z = empty_operand();
    return instr;
}

MIPS_instruction MIPS_mk2(MIPS_opcode op, MIPS_operand x, MIPS_operand y)
{
    MIPS_instruction instr;
    instr.opcode = op;
    instr.x = x;
    instr.y = y;
    instr.z = empty_operand();
    return instr;
}

MIPS_instruction MIPS_mk3(MIPS_opcode op, MIPS_operand x, MIPS_operand y,
        MIPS_operand z)
{
    MIPS_instruction instr;
    instr.opcode = op;
    instr.x = x;
    instr.y = y;
    instr.z = z;
    return instr;
}



int MIPS_write_operand(char* buffer, MIPS_operand operand)
{
    switch (operand.type)
    {
    case MIPS_SYMBOL:
        return sprintf(buffer, "%s", operand.symbol);
        break;

    case MIPS_OFFSET:
    case MIPS_IMM_32:
        return sprintf(buffer, "%d", operand.imm32);
        break;

    case MIPS_IMM_8:
        return sprintf(buffer, "%d", (int) operand.imm8);
        break;

    case MIPS_REG:
        switch (operand.reg)
        {
        case 0: return sprintf(buffer, "$zero");
        case 1: return sprintf(buffer, "$at");
        case 28: return sprintf(buffer, "$gp");
        case 29: return sprintf(buffer, "$sp");
        case 30: return sprintf(buffer, "$fp");
        case 31: return sprintf(buffer, "$ra");
        default:
        {
            int r = operand.reg;
            if (r >= 2 && r <= 3)
                return sprintf(buffer, "$v%d", r - 2);
            else if (r >= 4 && r <= 7)
                return sprintf(buffer, "$a%d", r - 4);
            else if (r >= 8 && r <= 15)
                return sprintf(buffer, "$t%d", r - 8);
            else if (r >= 16 && r <= 23)
                return sprintf(buffer, "$s%d", r - 16);
            else if (r >= 24 && r <= 25)
                return sprintf(buffer, "$t%d", r - 16);
            else if (r >= 26 && r <= 27)
                return sprintf(buffer, "$k%d", r - 26);
            else
                assert(0 && "MIPS_write_operand: invalid operand type");
        }
        break;
        }
        break;
    }
    return -1;
}


char* MIPS_write_instr(char* buffer, MIPS_instruction instr)
{
    char* p = buffer;
    p += sprintf(buffer, "    %s ", MIPS_op_name(instr.opcode));
    switch (instr.opcode)
    {
    case MIPS_add:
    case MIPS_sub:
    case MIPS_addi:
    case MIPS_and:
    case MIPS_andi:
    case MIPS_or:
    case MIPS_ori:
    case MIPS_slt:
    case MIPS_slti:
    case MIPS_sll:
    case MIPS_srl:
    case MIPS_bgt:
    case MIPS_bge:
    case MIPS_blt:
    case MIPS_ble:
    case MIPS_beq:
    case MIPS_bne:
        p += MIPS_write_operand(p, instr.x);
        p += sprintf(p, ", ");
        p += MIPS_write_operand(p, instr.y);
        p += sprintf(p, ", ");
        p += MIPS_write_operand(p, instr.z);
        break;

    case MIPS_mult:
    case MIPS_div:
    case MIPS_li:
        p += MIPS_write_operand(p, instr.x);
        p += sprintf(p, ", ");
        p += MIPS_write_operand(p, instr.y);
        break;

    case MIPS_lw:
    case MIPS_lb:
    case MIPS_la:
    case MIPS_sw:
    case MIPS_sb:
        p += MIPS_write_operand(p, instr.x);
        p += sprintf(p, ", ");
        p += MIPS_write_operand(p, instr.z);
        p += sprintf(p, "(");
        p += MIPS_write_operand(p, instr.y);
        p += sprintf(p, ")");
        break;

    case MIPS_mfhi:
    case MIPS_mflo:
    case MIPS_jal:
    case MIPS_jr:
    case MIPS_b:
        MIPS_write_operand(p, instr.x);
        break;
    }
    return buffer;
}


void MIPS_free_instr(MIPS_instruction* instruction)
{
    assert(instruction && "MIPS_free_instr: NULL argument passed");
    MIPS_free_operand(&instruction->x);
    MIPS_free_operand(&instruction->y);
    MIPS_free_operand(&instruction->z);
}
