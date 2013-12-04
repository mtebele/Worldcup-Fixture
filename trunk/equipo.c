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
	abb_t* plantel_nombre;
	size_t cant_jugadores;
};

/***************************************************
		IMPLEMENTACION PRIMITIVAS DE EQUIPO
****************************************************/

equipo_t *equipo_crear(char* nombre)
{
	equipo_t *equipo = malloc(sizeof(equipo_t));
	if (!equipo) return NULL;
	equipo->plantel = malloc(sizeof(jugador_t*) * MAX_JUG);
	if (!equipo->plantel) {
		free(equipo);
		return NULL;
	}
	equipo->plantel_nombre = abb_crear(strcmp, NULL);
	if (!equipo->plantel_nombre) {
		free(equipo->plantel);
		free(equipo);
		return NULL;		
	}
	equipo->nombre = strdup(nombre);
	equipo->cant_jugadores = 0;
	return equipo;
}

bool equipo_inscribir(equipo_t *equipo, jugador_t *jugador)
{
	if (!equipo || equipo->cant_jugadores == MAX_JUG)
		return false;
	equipo->plantel[equipo->cant_jugadores++] = jugador;
	char* nombre = jugador_nombre(jugador);
	abb_guardar(equipo->plantel_nombre, nombre, jugador);
	free(nombre);
	return true;
}

char* equipo_nombre(equipo_t *equipo)
{
	return strdup(equipo->nombre);
}

void equipo_agregar_gol(equipo_t *equipo, int dorsal)
{
	jugador_t *jugador = equipo->plantel[dorsal-1];
	jugador_agregar_gol(jugador);
}

jugador_t** equipo_plantel(equipo_t *equipo)
{
	//En el peor de los casos, sacarlo como const
	return equipo->plantel;
}

abb_t* equipo_plantel_nombre(equipo_t *equipo)
{
	return equipo->plantel_nombre;
}

void equipo_destruir(void *equipo)
{
	equipo_t *equipo_dest = (equipo_t*)equipo;
	free(equipo_dest->plantel);
	abb_destruir(equipo_dest->plantel_nombre);
	free(equipo_dest->nombre);
	free(equipo_dest);
}
