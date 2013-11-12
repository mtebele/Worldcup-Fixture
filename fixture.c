#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "fixture.h"

/***************************************************
			IMPLEMENTACIÓN TIPOS DE DATOS
****************************************************/

struct fixture {
	partido_t** partidos;
	size_t cantidad;
};

/***************************************************
		IMPLEMENTACION PRIMITIVAS DE JUGADOR
****************************************************/

int idrtopos(const char *idr, size_t n)
{
printf("idr: %s | cantidad: %d\n", idr, n);
	int instancia = idr[0] - '0';
printf("instancia: %d\n", instancia);
	if (instancia == 1) return n-1;
	int inicio_instancia = n + 1 - 2*instancia;
printf("inicio_instancia: %d\n", inicio_instancia);
	int letra = idr[1] - 'a';
	int pos = inicio_instancia + letra;
printf("pos: %d\n", pos);
	return pos;
}

fixture_t *fixture_crear(size_t cantidad)
{
	fixture_t *fixture = malloc(sizeof(fixture_t));
	if (!fixture) return NULL;
	fixture->partidos = malloc(sizeof(partido_t*) * cantidad);
	if (!fixture->partidos) {
		free(fixture);
		return NULL;
	}
	fixture->cantidad = 0;
	return fixture;
}

bool fixture_cargar(fixture_t* fixture, lista_t* lista)
{
	lista_iter_t* iter = lista_iter_crear(lista);
	partido_t* partido;
	while (!lista_iter_al_final(iter)) {
		if (fixture->cantidad % 2 == 0) {
			partido = partido_crear();
			if (!partido) return false;
			partido_agregar_local(partido, lista_iter_ver_actual(iter));
		}
		else
			partido_agregar_visitante(partido, lista_iter_ver_actual(iter));
		
		fixture->cantidad++;
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	return true;
}

partido_t* fixture_partido(fixture_t *fixture, char* idr, size_t cantidad)
{
	int pos = idrtopos(idr, cantidad - 1);
printf("pos: %d\n", pos);
	return fixture->partidos[pos];
}

//devuelve el partido siguiente que le toca jugar
partido_t* fixture_clasificar_equipo(fixture_t *fixture, char* idr, size_t cantidad)
{
	int pos = idrtopos(idr, cantidad - 1);
	if (pos % 2 == 1)
		pos--;
	int offset = (cantidad - pos) / 2;
	pos += offset;	
	return fixture->partidos[pos];
}

bool fixture_final(fixture_t *fixture, char* idr, size_t cantidad)
{
	int pos = idrtopos(idr, cantidad - 1);
	if (pos == cantidad)
		return true;
	return false;
}

void fixture_destruir(fixture_t *fixture)
{
	if (!fixture) return;	
	free(fixture);
}
