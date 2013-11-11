#ifndef JUGADOR_H
#define JUGADOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/*****************************************************
          DEFINICION DE LOS TIPOS DE DATOS
 ****************************************************/

/* El jugador está interpretado como una estructura que
* posee los campos identificadores (dorsal y nombre),
* más la cantidad de goles */

typedef struct jugador jugador_t;

/******************************************************
                PRIMITIVAS DE JUGADOR
******************************************************/

// Crea un jugador nuevo.
// Post: devuelve un jugador con nombre y dorsal pasados.
// El mismo no tiene goles.
jugador_t *jugador_crear(char* nombre, char* equipo, int dorsal);

// Obtiene el equipo del jugador
// Pre: El jugador fue creado.
// Post: Devuelve el equipo del jugador.
char* jugador_equipo(jugador_t *jugador);

// Obtiene el dorsal del jugador
// Pre: El jugador fue creado.
// Post: Devuelve el dorsal del jugador.
int jugador_dorsal(jugador_t *jugador);

// Obtiene el nombre del jugador.
// Pre: El jugador fue creado.
// Post: Devuelve el nombre del jugador.
char* jugador_nombre(jugador_t *jugador);

// Obtiene los goles del jugador
// Pre: El jugador fue creado.
// Post: Devuelve los goles del jugador.
int jugador_goles(jugador_t *jugador);

// Agrega goles al historial del jugador.
// Pre: El jugador fue creado, goles es un número mayor a 0.
// Post: Se suman los goles pasados al historial del jugador.
void jugador_agregar_goles(jugador_t *jugador, int goles); /*o bool?*/

// Destruye el jugador.
// Pre: El jugador fue creado.
// Post: El jugador es destruido.
void jugador_destruir(jugador_t *jugador);

#endif // JUGADOR_H
