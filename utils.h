#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <ctype.h>
#include <string.h>

// Clona una cadena con memoria dinámica
char *strdup(const char *s);

// Convierte int a char*
void itoa(char *buf, int base, int d);

// Devuelve true si el char es un número, false caso contrario
bool isNumeric(char c);

// Transforma el texto a mayúsculas
void upcase(char* str);

// Elimina los espacios de adelante y atrás de un texto
char* trim(char* str);

#endif // UTILS_H
