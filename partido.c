#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "partido.h"

/***************************************************
			IMPLEMENTACIÓN TIPOS DE DATOS
****************************************************/

struct partido {
	bool jugado;
	char *local;
	char *visitante;
	int goles_local;
	int goles_visitante;
};

/***************************************************
		IMPLEMENTACION PRIMITIVAS DE JUGADOR
****************************************************/

// Clona una cadena con memoria dinámica.
char *strdup(const char *s)
{
    char *n = malloc(strlen(s) + 1);
    if (n == NULL) return NULL;
    strcpy(n, s);
    return n;
}

partido_t *partido_crear()
{
	partido_t *partido = malloc(sizeof(partido_t));
	if (!partido) return NULL;
	partido->local = NULL;
	partido->visitante = NULL;
	partido->goles_local = -1;
	partido->goles_visitante = -1;
	partido->jugado = false;
	return partido;
}

bool partido_agregar_local(partido_t *partido, char *equipo)
{
	if (!partido) return false;
	if (!partido->local) {
		partido->local = equipo; //strdup?
		return true;
	}
	return false;
}

bool partido_agregar_visitante(partido_t *partido, char *equipo)
{
	if (!partido) return false;
	if (!partido->visitante) {
		partido->visitante = equipo; //strdup?
		return true;
	}
	return false;
}

bool partido_simular(partido_t *partido, int goles_local, int goles_visita)
{
	if (partido->jugado) return false;
	partido->goles_local = goles_local;
	partido->goles_visitante = goles_visita;
	partido->jugado = true;
	return true;
}

bool partido_realizado(partido_t *partido)
{
	return partido->jugado;
}

char* partido_ganador(partido_t *partido)
{
	if (!partido || !partido->jugado) return NULL;
	int resultado = partido->goles_local - partido->goles_visitante;
	if (resultado > 0)
		return partido->local;
	return partido->visitante;
}

char* partido_local(partido_t *partido)
{
	return partido->local;
}

char* partido_visitante(partido_t *partido)
{
	return partido->visitante;
}

int partido_goles_local(partido_t *partido)
{
	return partido->goles_local;
}

int partido_goles_visitante(partido_t *partido)
{
	return partido->goles_visitante;
}

void partido_destruir(partido_t *partido)
{
	if (!partido) return;	
	free(partido);
}
