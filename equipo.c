#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "equipo.h"

#define MAX_JUG 23

/***************************************************
			IMPLEMENTACIÓN TIPOS DE DATOS
****************************************************/

struct equipo {
	char* nombre;
	jugador_t** plantel;
	size_t cant_jugadores;
};

/***************************************************
		IMPLEMENTACION PRIMITIVAS DE EQUIPO
****************************************************/

// Clona una cadena con memoria dinámica
/*char *strdup(const char *s)
{
    char *n = malloc(strlen(s) + 1);
    if(n == NULL) return NULL;
    strcpy(n, s);
    return n;
}*/

equipo_t *equipo_crear(char* nombre)
{
	equipo_t *equipo = malloc(sizeof(equipo_t));
	if (!equipo) return NULL;
	equipo->plantel = malloc(sizeof(jugador_t*) * MAX_JUG);
	if (!equipo->plantel) {
		free(equipo);
		return NULL;
	}
	equipo->nombre = strdup(nombre);
	equipo->cant_jugadores = 0;
	return equipo;
}

bool equipo_inscribir(equipo_t *equipo, jugador_t *jugador)
{
	int cant_jugadores = equipo->cant_jugadores;
	if (!equipo || cant_jugadores == MAX_JUG)
		return false;
	equipo->plantel[cant_jugadores] = jugador;
	return true;
}

char* equipo_nombre(equipo_t *equipo)
{
	return equipo->nombre;
}

void equipo_agregar_gol(equipo_t *equipo, int dorsal)
{
	jugador_t *jugador = equipo->plantel[dorsal-1];
	jugador_agregar_gol(jugador);
}

void equipo_destruir(equipo_t *equipo)
{
	for (int i = 0; i < equipo->cant_jugadores; i++)
		jugador_destruir(equipo->plantel[i]);

	free(equipo->plantel);
	free(equipo->nombre);
	free(equipo);
}

jugador_t** equipo_plantel(equipo_t *equipo)
{
	return equipo->plantel;
}
