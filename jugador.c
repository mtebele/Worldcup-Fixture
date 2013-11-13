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
	return strdup(jugador->equipo);
}

int jugador_dorsal(jugador_t *jugador)
{
	return jugador->dorsal;
}

char* jugador_nombre(jugador_t *jugador)
{
	return strdup(jugador->nombre);
}

int jugador_goles(jugador_t *jugador)
{
	return jugador->goles;
}

void jugador_agregar_gol(jugador_t *jugador)
{
	printf("GOLAZO DE: %s\n", jugador_nombre(jugador));
	jugador->goles++;
}

void jugador_destruir(void *jugador)
{
	if (!jugador) return;
	jugador_t *player = (jugador_t*)jugador; 	
	free(player->nombre);
	free(player->equipo);
	free(player);
}
