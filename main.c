#include <stdio.h>
#include "ast.h"
#include "parser.h"

#include "mips.h"


int main(int argc, char* argv[])
{
    node* root = parse_stdin();
    if (root == NULL)
    {
        fprintf(stderr, "Errors too severe to generate meaningful output,"
            "better luck next time. \nSorry!");
    }
    else
    {
        char buffer[8192];

        ir_code* code = build_ir(root);

        //sym_print_table(buffer, &code->table);
        //printf("%s\n", buffer);

        printf("-----------------------------------------------------\n");
        ir_list* c = code->codes;

        printf(".data\n");
        int prev = IR_DATA;
        while (c)
        {
            ir_pretty_print(buffer, c->item);
            if (c->item.type == IR_DATA && prev != IR_DATA)
                printf(".data\n");
            else if (c->item.type != IR_DATA && prev == IR_DATA)
                printf(".text\n");

            if (c->item.type != IR_LABEL)
                printf("    %s\n", buffer);
            else
                printf("%s\n", buffer);
            prev = c->item.type;
            c = c->next;
        }
    }
    return 0;
}
