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
        line_list* list = ir_output(
            //ir_mkdata_string("dupa", "blea\n\rhhh\"")
            ir_mkenter("dupaF")
            //ir_mkgoto("kibel")
        );
        print_line_list(list);
        free_line_list(list);

        ir_code* code = build_ir(root);
    }
    return 0;
}
