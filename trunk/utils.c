#include <stdio.h>
#include "utils.h"

// Clona una cadena con memoria dinámica
char *strdup(const char *s)
{
    char *n = malloc(strlen(s) + 1);
    if(n == NULL) return NULL;
    strcpy(n, s);
    return n;
}

bool isNumeric(char c)
{
	return (c >= '0' && c <= '9');
}

void upcase(char* str)
{
	int len = strlen(str);
	for (int i = 0; i < len; i++)
		str[i] = toupper(str[i]);
}

char* trim(char* str)
{
	char* start = str;
	while (*start && isspace(*start))
		++start;
	char* i = start;
	char* end = start;
	while (*i)
		if(!isspace(*(i++)))
			end = i;
	*end = 0;
	return start;
}
