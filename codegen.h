#ifndef CODEGEN_H_
#define CODEGEN_H_

typedef enum
{
    ST_UNKNOWN,
    ST_REG,
    ST_LOCAL,
    ST_GLOBAL
} storage_type;


typedef struct
{
    storage_type storage;
    int size;
    union
    {
        int reg;
        int offset;
    };
} codegen_info;


#endif // CODEGEN_H_
