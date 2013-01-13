#ifndef PARSER_H_
#define PARSER_H_

#include "ast.h"
#include "ir.h"
#include "symbol_table.h"

/*
 * Returns root node of the whole program, or NULL if the parsing has failed
 * severely enough.
 */
node* parse_stdin();

typedef struct
{
    ir_list* codes;
    symbol_table table;
} ir_code;

ir_code* build_ir(node* ast);


typedef struct line_list
{
    char* line;
    struct line_list* next;
} line_list;


line_list* empty_line_list(void);
line_list* single_line(char* line);
line_list* append_line(line_list* last, char* line);
void print_line_list(line_list* lines);
void free_line_list(line_list* list);

void next_label(char* buffer);

line_list* ir_output(ir_item item);


#endif // PARSER_H_
