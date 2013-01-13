#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"

ir_list* ir_empty_list(void)
{
    ir_list* node = malloc(sizeof(ir_list));
    node->next = NULL;
    return node;
}

ir_list* ir_single_code(ir_item item)
{
    ir_list* node = ir_empty_list();
    node->item = item;
    return node;
}

ir_list* ir_append_item(ir_list* last, ir_item item)
{
    ir_list* node = ir_empty_list();
    node->item = item;
    if (last != NULL)
    {
        while (last->next != NULL)
            last = last->next;
        last->next = node;
    }
    return node;
}

void ir_free_list(ir_list* codes)
{
    while (codes != NULL)
    {
        ir_free_item(&codes->item);
        ir_list* tmp = codes;
        codes = codes->next;
        free(tmp);
    }
}

