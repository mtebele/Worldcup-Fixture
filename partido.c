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
	char *nombre_local;
	char *nombre_visitante;
	int goles_local;
	int goles_visitante;
};

/***************************************************
		IMPLEMENTACION PRIMITIVAS DE PARTIDO
****************************************************/

partido_t *partido_crear()
{
	partido_t *partido = malloc(sizeof(partido_t));
	if (!partido) return NULL;
	partido->nombre_local = NULL;
	partido->nombre_visitante = NULL;
	partido->goles_local = -1;
	partido->goles_visitante = -1;
	partido->jugado = false;
	return partido;
}

bool partido_agregar_local(partido_t *partido, char *equipo)
{
	if (!partido) return false;
	if (!partido->nombre_local) {
		partido->nombre_local = strdup(equipo);
		if (!partido->nombre_local) return false;
		return true;
	}
	return false;
}

bool partido_agregar_visitante(partido_t *partido, char *equipo)
{
	if (!partido) return false;
	if (!partido->nombre_visitante) {
		partido->nombre_visitante = strdup(equipo);
		if (!partido->nombre_local) return false;
		return true;
	}
	return false;
}

bool partido_hay_clasificados(partido_t *partido)
{
	if (!partido) return false;
	bool hay_local = (partido->nombre_local != NULL);
	bool hay_visita = (partido->nombre_visitante != NULL);
	return (hay_local && hay_visita);
}

bool partido_jugar(partido_t *partido, int goles_local, int goles_visita)
{
	if (!partido || partido->jugado) return false;
	partido->goles_local = goles_local;
	partido->goles_visitante = goles_visita;
	partido->jugado = true;
	return true;
}

char* partido_ganador(partido_t *partido)
{
	if (!partido || !partido->jugado) return NULL;
	int resultado = partido->goles_local - partido->goles_visitante;
	if (resultado > 0)
		return partido_nombre_local(partido);
	return partido_nombre_visitante(partido);
}

char* partido_nombre_local(partido_t *partido)
{
	if (!partido) return NULL;	
	return partido->nombre_local;
}

char* partido_nombre_visitante(partido_t *partido)
{
	if (!partido) return NULL;	
	return partido->nombre_visitante;
}

int partido_goles_local(partido_t *partido)
{
	if (!partido) return -1;
	return partido->goles_local;
}

int partido_goles_visitante(partido_t *partido)
{
	if (!partido) return -1;
	return partido->goles_visitante;
}

bool partido_jugado(partido_t *partido)
{
	if (!partido) return false;
	return partido->jugado;
}

void partido_destruir(partido_t *partido)
{
	if (!partido) return;
	if (partido->nombre_local)
		free(partido->nombre_local);
	if (partido->nombre_visitante)
		free(partido->nombre_visitante);
	free(partido);
}
