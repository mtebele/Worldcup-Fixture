#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "jugador.h"

/***************************************************
			IMPLEMENTACIÓN TIPOS DE DATOS
****************************************************/

struct jugador {
	char *nombre;
	char *equipo;
	int dorsal;
    int goles;
};

/***************************************************
		IMPLEMENTACION PRIMITIVAS DE JUGADOR
****************************************************/

jugador_t *jugador_crear(char *nombre, char *equipo, int dorsal)
{
	jugador_t *jugador = malloc(sizeof(jugador_t));
	if (!jugador) return NULL;
	jugador->nombre = strdup(nombre);
	jugador->equipo = strdup(equipo);
	jugador->dorsal = dorsal;
	jugador->goles = 0;

	return jugador;
}

char* jugador_equipo(jugador_t *jugador)
{
	return jugador->equipo; //strdup?
}

int jugador_dorsal(jugador_t *jugador)
{
	return jugador->dorsal;
}

char* jugador_nombre(jugador_t *jugador)
{
	return jugador->nombre; //strdup?
}

int jugador_goles(jugador_t *jugador)
{
	return jugador->goles;
}

void jugador_agregar_gol(jugador_t *jugador)
{
	jugador->goles++;
}

void jugador_destruir(jugador_t *jugador)
{
	if (!jugador) return;	
	free(jugador->nombre);
	free(jugador->equipo);
	free(jugador);
}
