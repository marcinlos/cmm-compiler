#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "mips.h"

MIPS_list* MIPS_empty_list(void)
{
    MIPS_list* node = malloc(sizeof(MIPS_list));
    node->next = NULL;
    return node;
}

MIPS_list* MIPS_single_code(MIPS_instruction item)
{
    MIPS_list* node = MIPS_empty_list();
    node->instr = item;
    return node;
}

MIPS_list* MIPS_append_item(MIPS_list* last, MIPS_instruction item)
{
    MIPS_list* node = MIPS_empty_list();
    node->instr = item;
    if (last != NULL)
    {
        while (last->next != NULL)
            last = last->next;
        last->next = node;
    }
    return node;
}

void MIPS_free_list(MIPS_list* codes)
{
    while (codes != NULL)
    {
        MIPS_free_instr(&codes->instr);
        MIPS_list* tmp = codes;
        codes = codes->next;
        free(tmp);
    }
}

