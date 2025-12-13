#include <string.h>
#include <stdlib.h>

#include "utils.h"

char* str_slice(char* start, char* end)
{
	char *slice = malloc(end - start + 1);
    strncpy(slice, start, end - start);
    slice[end - start] = '\0';
    return slice;
}

int is_delim(char n)
{
    return n == '{' || n == '}' || n == '(' || n == ')' || n == '[' || n == ']'
        || n == ',' || n == '-' || n == '+' || n == '*' || n == '/'
        || n == ';';
}
