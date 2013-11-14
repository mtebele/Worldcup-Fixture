#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "mensajes.h"

/****************************************
	IMPLEMENTACION DE LOS MENSAJES
****************************************/

void mensaje_formato_invalido() {
	printf("ERROR. Formato inválido.\n");
}

void mensaje_error() {
	printf("ERROR\n");
}

void mensaje_ok() {
	printf("OK\n");
}

void mensaje_resultado_ya_existente(char* idr) {
    printf("Error: el resultado con id %s ya existe\n", idr);
}

void mensaje_resultado_no_existente(char* idr) {
    printf("Error: el resultado con id %s no existe\n", idr);
}

void mensaje_equipo_ya_inscripto(char* equipo) {
    printf("Error: el equipo %s ya esta inscipto en el fixture\n", equipo);
}

void mensaje_equipo_no_inscripto(char* equipo) {
    printf("Error: el equipo %s no esta inscipto en el fixture\n", equipo);
}

void mensaje_jugador_no_inscripto(char* jugador) {
    printf("Error: el jugador %s no esta inscipto en el fixture\n", jugador);
}

void mensaje_listar_jugadores(char* jugador, int dorsal, int goles) {
    printf("%s,%d: Goles: %d\n", jugador, dorsal, goles);
}

void mensaje_listar_goleador(char* jugador, char* equipo, int goles) {
    printf("%s: %s Goles: %d\n", jugador, equipo, goles);
}

void mensaje_goles_jugador(char* jugador, int dorsal, char* equipo, int goles) {
    printf("%s,%d: %s Goles:%d\n", jugador, dorsal, equipo, goles);
}

void mensaje_mostrar_resultado(char* equipo1, int goles1, char* equipo2, int goles2) {
    printf("resultado: %s:%d vs %s:%d\n", equipo1, goles1, equipo2, goles2);
}
