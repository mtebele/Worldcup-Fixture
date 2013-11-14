#ifndef SISTEMA_H
#define SISTEMA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "abb.h"
#include "hash.h"
#include "lista.h"
#include "heap.h"
#include "lista.h"
#include "equipo.h"
#include "jugador.h"
#include "partido.h"
#include "fixture.h"
#include "utils.h"

/******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 ******************************************************************/

typedef struct sistema sistema_t;
typedef enum {OK, RESULTADO_NOEXISTE, RESULTADO_YAEXISTE, ERROR} resultado_t;
	      
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

// Agrega un resultado al torneo.
// Pre: El sistema fue creado.
// Post: Agrega el resultado al fixture del sistema, actualizando la información
// de goleadores y clasificados a la próxima ronda. Devuelve un resultado que
// indica si la operación fue exitosa.
resultado_t sistema_agregar_resultado(sistema_t* sistema, char* vec_parametros[]);

// Genera una lista de nombres correspondientes a los jugadores del equipo con
// nombre "nombre", ordenada bajo el criterio recibido.
// Pre: El sistema fue creado.
// Post: Devuelve la lista bajo las condiciones indicadas en la especificación.
// NULL si el modo es inválido o el equipo no existe en el sistema. 
lista_t* sistema_listar_jugadores(sistema_t* sistema, char* modo, char* nombre);

// Obtiene el goleador del torneo.
// Pre: El sistema fue creado.
// Devuelve el nombre del goleador, su dorsal, su equipo y la cantidad de goles.
char** sistema_listar_goleador(sistema_t* sistema);

// Obtiene información sobre el jugador consultado.
// Pre: El sistema fue creado.
// Post: Devuelve nombre, equipo, dorsal y cantidad de goles del jugador consul-
// tado. NULL en caso de que no exista el jugador en el sistema.
char** sistema_goles_jugador(sistema_t* sistema, char* nombre);

// Obtiene información del resultado correspondiente al partido identificado con 
// el idr recibido.
// Pre: El sistema fue creado.
// Post: Devuelve equipos involucrados junto con los goles anotados por cada uno
// NULL si el idr no corresponde a un partido válido.
char** sistema_mostrar_resultado(sistema_t* sistema, char* idr);

// Agrega un equipo al sistema.
// Pre: El sistema fue creado.
// Post: Agrega el equipo con el nombre recibido. Devuelve true si logra agregar
// el equipo, false en caso contrario.
bool sistema_agregar_equipo(sistema_t* sistema, char* nombre);

// Agrega un jugador al sistema.
// Pre: El sistema fue creado.
// Post: Agrega al jugador con nombre y dorsal recibidos, inscribiéndolo en el
// equipo recibido. Devuelve true si la operación es exitosa, si no, false.
bool sistema_agregar_jugador(sistema_t* sistema, int dorsal, char* equipo, char* nombre);

// Determina la cantidad de equipos participantes en el torneo.
// Pre: El sistema fue creado.
// Post: Devuelve la cantidad de equipos particpantes en el torneo.
size_t sistema_cantidad_equipos(sistema_t *sistema);

// Arma el fixture del sistema estableciendo las llaves iniciales.
// Pre: El sistema fue creado.
// Post: Agrupa de a dos los equipos recibidos en la lista en partidos
// eliminatorios y carga los partidos en el fixture del sistema.
// Devuelve true si la operación es exitosa, false en caso contrario.
bool sistema_cargar_fixture(sistema_t* sistema, lista_t* lista);

// Destruye el sistema.
// Pre: El sistema fue creado.
// Post: El sistema es destruido.
void sistema_destruir(sistema_t* sistema);

#endif // SISTEMA_H
