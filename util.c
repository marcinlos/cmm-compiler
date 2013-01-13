#include <string.h>
#include <stdlib.h>


int calc_escaped_size(const char* text)
{
    int n = 0;
    while (*text)
    {
        switch(*text++)
        {
        case '\t': n += 2; break;
        case '\n': n += 2; break;
        case '\"': n += 2; break;
        case '\\': n += 2; break;
        case '\r': n += 2; break;
        default: ++ n; break;
        }
    }
    return n;
}

char* escape_string(char* dest, const char* src)
{
    char* p = dest;
    while (*src)
    {
        switch(*src)
        {
        case '\t': strcpy(p, "\\t"); p += 2; break;
        case '\n': strcpy(p, "\\n"); p += 2; break;
        case '\"': strcpy(p, "\\\""); p += 2; break;
        case '\\': strcpy(p, "\\\\"); p += 2; break;
        case '\r': strcpy(p, "\\r"); p += 2; break;
        default: *p ++ = *src; break;
        }
        ++ src;
    }
    return dest;
}

char* escape_alloc(const char* text)
{
    char* buffer = malloc(calc_escaped_size(text));
    return escape_string(buffer, text);
}
