#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "jugador.h"

/***************************************************
			IMPLEMENTACIÓN TIPOS DE DATOS
****************************************************/

struct jugador {
	char* nombre;
	int dorsal;
    int goles;
};

/***************************************************
		IMPLEMENTACION PRIMITIVAS DE JUGADOR
****************************************************/

jugador_t *jugador_crear(char* nombre, int dorsal)
{
	jugador_t *jugador = malloc(sizeof(jugador_t));
	if (!jugador) return NULL;
	jugador->nombre = strdup(nombre);
	jugador->dorsal = dorsal;
	jugador->goles = 0;

	return jugador;
}

int jugador_dorsal(jugador_t *jugador)
{
	return jugador->dorsal;
}

char* jugador_nombre(jugador_t *jugador)
{
	return jugador->nombre; //strdup?
}

void jugador_agregar_goles(jugador_t *jugador, int goles)
{
	jugador->goles += goles;
}

void jugador_destruir(jugador_t *jugador)
{
	if (!jugador) return;	
	free(jugador->nombre);
	free(jugador);
}
