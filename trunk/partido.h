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
// Post: devuelve un partido.
partido_t *partido_crear();

// Agrega equipo local al partido.
// Pre: El partido fue creado.
// Post: Devuelve true si agrega el equipo, false en caso contrario
bool partido_agregar_local(partido_t *partido, char *eq);

// Agrega equipo visitante al partido.
// Pre: El partido fue creado.
// Post: Devuelve true si agrega el equipo, false en caso contrario
bool partido_agregar_visitante(partido_t *partido, char *eq);

// Simula el resultado de un partido
// Pre: El partido fue creado
// Post: Marca el partido como realizado y actualiza la información del mismo.
bool partido_simular(partido_t *partido, int gloc, int gvis);

// Determina si un partido se jugó o no.
// Pre: el partido fue creado.
// Post: devuelve true si el partido se jugó, false en
// caso contrario.
bool partido_realizado(partido_t *partido); 

// Obtiene el ganador del partido
// Pre: el partido fue creado
// Post: devuelve el nombre del equipo ganador
char* partido_ganador(partido_t *partido);

// Obtiene el local del partido
// Pre: el partido fue creado
// Post: devuelve el nombre del equipo local
char* partido_local(partido_t *partido);

// Obtiene el visitante del partido
// Pre: el partido fue creado
// Post: devuelve el nombre del equipo visitante
char* partido_visitante(partido_t *partido);

// Obtiene los goles del local
// Pre: El partido fue creado.
// Post: Devuelve la cantidad de goles del equipo local.
int partido_goles_local(partido_t *partido);

// Obtiene los goles del visitante.
// Pre: El partido fue creado.
// Post: Devuelve la cantidad de goles del equipo visitante.
int partido_goles_visitante(partido_t *partido);

// Destruye el partido.
// Pre: El partido fue creado.
// Post: El partido es destruido.
void partido_destruir(partido_t *partido);

#endif // PARTIDO_H
