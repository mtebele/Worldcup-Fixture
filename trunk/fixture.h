#ifndef FIXTURE_H
#define FIXTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "partido.h"
#include "lista.h"

/*****************************************************
          DEFINICION DE LOS TIPOS DE DATOS
 ****************************************************/

typedef struct fixture fixture_t;

/******************************************************
                PRIMITIVAS DE JUGADOR
******************************************************/

fixture_t *fixture_crear(size_t cantidad);

bool fixture_cargar(fixture_t* fixture, lista_t* lista);

partido_t* fixture_partido(fixture_t *fixture, char* idr, size_t cantidad);

partido_t* fixture_clasificar_equipo(fixture_t *fixture, char* idr, size_t cantidad);

bool fixture_final(fixture_t *fixture, char* idr, size_t cantidad);

void fixture_destruir(fixture_t *fixture);

#endif // FIXTURE_H
