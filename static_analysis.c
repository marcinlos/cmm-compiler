#include <stdio.h>
#include "parser.h"

static int label_counter = 100;


void next_label(char* buffer)
{
    sprintf(buffer, "__L%d__", label_counter++);
}


