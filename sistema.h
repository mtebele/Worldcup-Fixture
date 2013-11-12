#ifndef SISTEMA_H
#define SISTEMA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "abb.h"
#include "hash.h"
#include "lista.h"
#include "heap.h"
#include "lista.h"
#include "equipo.h"
#include "jugador.h"
#include "partido.h"
#include "fixture.h"

/******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 ******************************************************************/

typedef struct sistema sistema_t;
typedef enum {OK, EQUIPO_NOINSCRIPTO, NONE} resultado_t;

// Clona una cadena con memoria dinámica
char *strdup(const char *s);
		      
// Determinará el criterio de comparación entre claves.
// Pre: las cadenas fueron creadas.
// Post: devuelve un valor negativo si la primera clave es mayor, cero si
// son iguales y positivo si el segundo es mayor.
typedef int (*sistema_comparar_clave_t) (const char *, const char *);

// Destruye un dato.
// Pre: el dato fue creado.
// Post: Destruye el dato.
typedef void (*sistema_destruir_dato_t) (void *);

/******************************************************************
 *                    PRIMITIVAS DEL SISTEMA
 ******************************************************************/

// Crea un sistema.
// Post: devuelve un nuevo sistema vacío.
sistema_t* sistema_crear();

resultado_t sistema_agregar_resultado(sistema_t* sistema, char* vec_parametros[]);

lista_t* sistema_listar_jugadores(sistema_t* sistema, char* vec_parametros[]);

char** sistema_listar_goleador(sistema_t* sistema);

char** sistema_goles_jugador(sistema_t* sistema, char* nombre);

char* sistema_mostrar_resultado(sistema_t* sistema, char* idr);

bool sistema_agregar_equipo(sistema_t* sistema, char* nombre);

bool sistema_agregar_jugador(sistema_t* sistema, int dorsal, char* equipo, char* nombre);

// Determina la cantidad de equipos participantes en el torneo
// Pre: El sistema fue creado
// Post: Devuelve la cantidad de equipos particpantes en el torneo
size_t sistema_cantidad_equipos(sistema_t *sistema);

bool sistema_cargar_fixture(sistema_t* sistema, lista_t* lista);

void sistema_destruir(sistema_t* sistema);

#endif // SISTEMA_H
