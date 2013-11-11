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
	char *loc;
	char *vis;
	int goles_loc;
	int goles_vis;

};

/***************************************************
		IMPLEMENTACION PRIMITIVAS DE JUGADOR
****************************************************/

partido_t *partido_crear(const char *idr)
{
	partido_t *partido = malloc(sizeof(partido_t));
	if(!partido) return NULL;
	partido->idr = strdup(idr);
	if(!partido->idr)
	{
		free(partido);
		return NULL;
	}
	partido->loc = NULL;
	partido->vis = NULL;
	partido->goles_loc = -1;
	partido->goles_vis = -1;
	partido->jugado = false;
	return partido;
}

bool partido_agregar_local(partido_t *partido, char *eq);
{
	if(!partido) return false;
	if(!partido->loc)
	{
		partido->loc = eq; //strdup?
		return true;
	}
	return false;
}

bool partido_agregar_visitante(partido_t *partido, char *eq);
{
	if(!partido) return false;
	if(!partido->vis)
	{
		partido->loc = eq; //strdup?
		return true;
	}
	return false;
}

bool partido_simular(partido_t *partido, int gloc, int gvis)
{
	if(partido->jugado) return false;
	partido->goles_loc = gloc;
	partido->goles_vis = gvis;
	partido->jugado = true;
	return true;
}

bool partido_realizado(partido_t *partido)
{
	return partido->jugado;
}

char* partido_ganador(partido_t *partido)
{
	if(!partido || !partido->jugado) return NULL;
	int result = partido->goles_loc - partido->goles_vis;
	if (result > 0) return partido->loc;
	return partido->vis;
}

int partido_gloc(partido_t *partido)
{
	return partido->goles_loc;
}

int partido_gvis(partido_t *partido)
{
	return partido->goles_vis;
}

void partido_destruir(partido_t *partido)
{
	if (!partido) return;	
	free(partido->idr);
	free(partido);
}
