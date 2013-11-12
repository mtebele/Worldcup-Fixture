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
	size_t tamanio;
};

/***************************************************
		IMPLEMENTACION PRIMITIVAS DE JUGADOR
****************************************************/

int idrtopos(const char *idr, size_t n)
{
	printf("hago idrtopos para %s\n", idr);
	int instancia = idr[0] - '0';
	if (instancia == 1) return n-1;
	int inicio_instancia = n + 1 - 2*instancia;
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
	fixture->tamanio = cantidad;
	return fixture;
}

bool fixture_cargar(fixture_t* fixture, lista_t* lista)
{
	lista_iter_t* iter = lista_iter_crear(lista);
	int i = 0;
	partido_t* partido;
	while (!lista_iter_al_final(iter)) {
		if (i % 2 == 0) {
			partido = partido_crear();
			if (!partido) return false;
			partido_agregar_local(partido, lista_iter_ver_actual(iter));
		}
		else{
			partido_agregar_visitante(partido, lista_iter_ver_actual(iter));
			fixture->partidos[fixture->cantidad] = partido;
			fixture->cantidad++;
		}
		i++;
		lista_iter_avanzar(iter);
	}
	
	lista_iter_destruir(iter);

	i = fixture_cantidad(fixture);
	/*creo el resto de los partidos*/
	while(i < fixture_tamanio(fixture))
	{
		partido = partido_crear();
		fixture->partidos[i] = partido;
		i++;
	}

	return true;
}

partido_t* fixture_partido(fixture_t *fixture, char* idr, size_t cantidad)
{
	int pos = idrtopos(idr, cantidad - 1);
	return fixture->partidos[pos];
}

//devuelve el partido siguiente que le toca jugar
partido_t* fixture_clasificar_equipo(fixture_t *fixture, char* idr, size_t cantidad)
{
	printf("INICIO CLASIFICAR EQUIPO\n");	
	int pos = idrtopos(idr, cantidad - 1);
	if (pos % 2 == 1)
		pos--;
	int offset = (cantidad - pos) / 2;
	pos += offset;	
	printf("la posición del próximo partido es %i\n",pos);	
	return fixture->partidos[pos];
}

bool fixture_final(fixture_t *fixture, char* idr, size_t cantidad)
{
	int pos = idrtopos(idr, cantidad - 1);
	if (pos == cantidad)
		return true;
	return false;
}

size_t fixture_cantidad(fixture_t *fixture)
{
	return fixture->cantidad;
}

size_t fixture_tamanio(fixture_t *fixture)
{
	return fixture->tamanio;
}

void fixture_destruir(fixture_t *fixture)
{
	if (!fixture) return;	
	free(fixture);
}
