#ifndef PARTIDO_H
#define PARTIDO_H

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

typedef struct partido partido_t;

/******************************************************
                PRIMITIVAS DE JUGADOR
******************************************************/

// Crea un partido nuevo.
// Post: devuelve un partido con la información pasada
partido_t *partido_crear(char *loc, int gloc, char *vis, int gvis);

// Obtiene el ganador del partido
// Pre: el partido fue creado
// Post: devuelve el nombre del equipo ganador
char* partido_ganador(partido_t *partido);

// Obtiene los goles del local
// Pre: El partido fue creado.
// Post: Devuelve la cantidad de goles del equipo local.
int partido_gloc(partido_t *partido);

// Obtiene el nombre del jugador.
// Pre: El jugador fue creado.
// Post: Devuelve el nombre del jugador.
char* jugador_nombre(jugador_t *jugador);

// Obtiene los goles del visitante.
// Pre: El partido fue creado.
// Post: Devuelve la cantidad de goles del equipo visitante.
int partido_gvis(partido_t *partido);

// Destruye el partido.
// Pre: El partido fue creado.
// Post: El partido es destruido.
void partido_destruir(jugador_t *jugador);

#endif // PARTIDO_H
