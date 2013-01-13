#ifndef UTIL_H_
#define UTIL_H_


int calc_escaped_size(const char* text);
char* escape_string(char* dest, const char* src);

/*
 * Allocates char buffer on its own, returned array needs to be manually
 * freed.
 */
char* escape_alloc(const char* text);

#endif // UTIL_H_
