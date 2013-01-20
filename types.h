#ifndef TYPES_H_
#define TYPES_H_

#include "ast.h"

typedef enum
{
    T_VOID,
    T_INT,
    T_CHAR,
    T_INT_ARRAY,
    T_CHAR_ARRAY
} basic_type;


typedef struct
{
    basic_type type;
    int elements;
} type;


type type_from_ast(value_type type);

int type_print(char* buffer, type t);


#endif // TYPES_H_
