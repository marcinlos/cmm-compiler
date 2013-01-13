#ifndef TYPES_H_
#define TYPES_H_


typedef enum
{
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


#endif // TYPES_H_
