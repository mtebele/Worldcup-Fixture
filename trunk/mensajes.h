#ifndef MENSAJES_H
#define MENSAJES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/****************************************
	MODULO PARA IMPRIMIR MENSAJES
****************************************/

void mensaje_formato_invalido();

void mensaje_error();

void mensaje_ok();

void mensaje_resultado_ya_existente(char* idr);

void mensaje_resultado_no_existente(char* idr);

void mensaje_equipo_ya_inscripto(char* equipo);

void mensaje_equipo_no_inscripto(char* equipo);

void mensaje_listar_jugadores(/*??*/);

void mensaje_listar_goleador(char* jug, char* equipo, int goles);

void mensaje_goles_jugador(char* jug, int dorsal, char* equipo, int goles);

void mensaje_mostrar_resultado(char* eq1, int gol1, char* eq2, int gol2);

#endif // MENSAJES_H
