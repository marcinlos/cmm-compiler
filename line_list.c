#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"

line_list* empty_line_list(void)
{
    line_list* node = malloc(sizeof(line_list));
    node->line = NULL;
    node->next = NULL;
    return node;
}

line_list* single_line(char* line)
{
    line_list* node = empty_line_list();
    node->line = strdup(line);
    return node;
}

line_list* append_line(line_list* last, char* line)
{
    line_list* node = empty_line_list();
    node->line = strdup(line);
    if (last != NULL)
    {
        while (last->next != NULL)
            last = last->next;
        last->next = node;
    }
    return node;
}

void print_line_list(line_list* lines)
{
    while (lines != NULL)
    {
        printf("%s\n", lines->line);
        lines = lines->next;
    }
}

void free_line_list(line_list* lines)
{
    while (lines != NULL)
    {
        free(lines->line);
        line_list* tmp = lines;
        lines = lines->next;
        free(tmp);
    }
}

