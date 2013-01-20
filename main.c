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
        /*line_list* list = ir_output(
            //ir_mkdata_string("dupa", "blea\n\rhhh\"")
            ir_mkenter("dupaF")
            //ir_mkgoto("kibel")
        );
        print_line_list(list);
        free_line_list(list);*/
        char buffer[8192];

        ir_code* code = build_ir(root);

        sym_print_table(buffer, &code->table);
        printf("%s\n", buffer);

        ir_list* c = code->codes;
        while (c)
        {
            ir_pretty_print(buffer, c->item);
            printf("    %s\n", buffer);
            c = c->next;
        }
    }
    return 0;
}
