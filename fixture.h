#ifndef FIXTURE_H
#define FIXTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "partido.h"
#include "lista.h"
#include "utils.h"

/*****************************************************
          DEFINICION DE LOS TIPOS DE DATOS
 ****************************************************/

typedef struct fixture fixture_t;

int obtener_instancia(const char *idr, size_t n);

/******************************************************
                PRIMITIVAS DE JUGADOR
******************************************************/

// Crea un fixture con la cantidad de partidos recibida.
// Post: Crea un fixture con tantos partidos como can-
// tidad recibida por parámetro. 
fixture_t *fixture_crear(size_t cantidad);

// Carga un fixture con los equipos recibidos en una lista.
// Pre: El fixture fue creado.
// Post: Agrega los equipos de la lista a los partidos
// de la fase inicial del fixture. Devuelve true si la
// operación es exitosa. False en caso contrario.
bool fixture_cargar(fixture_t* fixture, lista_t* lista);

// Obtiene el partido con identificador "idr" del fixture.
// Pre: El fixture fue creado.
// Post: Devuelve el partido con identificador "idr" del fixture.
partido_t* fixture_partido(fixture_t *fixture, char* idr, size_t cantidad);

// Obtiene la llave a la que clasifica el ganador del partido "idr".
// Pre: El fixture fue creado
// Devuelve la llave a la que clasifica el ganador del partido "idr". NULL si
// El idr es inválido. 
partido_t* fixture_clasificar_equipo(fixture_t *fixture, char* idr, size_t cantidad);

// Determina si un partido es final en el fixture.
// Pre: El sistema fue creado.
// Post: Devuelve true si el partido es la final, false en otro caso.
bool fixture_final(fixture_t *fixture, char* idr, size_t cantidad);

// Destruye el fixture.
// Pre: El fixture fue creado.
// Post: El fixture es destruido.
void fixture_destruir(fixture_t *fixture);

// Obtiene la cantidad de partidos del fixture.
// Pre: El fixture fue creado.
// Post: Devuelve la cantidad de partidos del fixture.
size_t fixture_cantidad(fixture_t *fixture);

// Obtiene la cantidad de partidos que puede guardar el fixture.
// Pre: El fixture fue creado.
// Post: Devuelve la cantidad de partidos que puede guardar el fixture.
size_t fixture_tamanio(fixture_t *fixture);

#endif // FIXTURE_H
