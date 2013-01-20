#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "ast.h"


type type_from_ast(value_type t)
{
    basic_type basic;
    switch (t)
    {
    case TYPE_VOID: basic = T_VOID; break;
    case TYPE_INT:  basic = T_INT;  break;
    case TYPE_CHAR: basic = T_CHAR; break;
    }
    type tt;
    tt.type = basic;
    return tt;
}

static const char* basic_type_string(basic_type b)
{
    switch (b)
    {
    case T_VOID:
        return "void";
    case T_INT:
    case T_INT_ARRAY:
        return "int";
    case T_CHAR:
    case T_CHAR_ARRAY:
        return "char";
    default:
        assert(0 && "basic_type_string: invalid basic type");
        return NULL;
    }
}

int type_is_array(type t)
{
    return t.type == T_INT_ARRAY || t.type == T_CHAR_ARRAY;
}

int type_print(char* buffer, type t)
{
    const char* b = basic_type_string(t.type);
    //assert(! type_is_array(t) && "type_print: array types not supported");
    char* p = buffer;
    p += sprintf(buffer, "%s", b);
    if (type_is_array(t))
    {
        p += sprintf(p, "[%d]", t.elements);
    }
    return p - buffer;
}
