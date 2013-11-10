#ifndef EQUIPO_H
#define EQUIPO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "jugador.h"

/*****************************************************
          DEFINICION DE LOS TIPOS DE DATOS
 ****************************************************/

/* El equipo está compuesto de un nombre identificador
y un vector de jugadores, cada posición del mismo es-
tará asociada al dorsal de cada jugador */

typedef struct equipo equipo_t;

/******************************************************
                PRIMITIVAS DE JUGADOR
******************************************************/

// Crea un equipo nuevo.
// Post: devuelve un equipo con el nombre pasado y sin
// jugadores.
equipo_t *equipo_crear(char* nombre);

// Inscribe un jugador al equipo.
// Pre: El equipo fue creado.
// Post: Devuelve true si logra inscribir al jugador.
// False en caso contrario.
bool equipo_inscribir(equipo_t *equipo, jugador_t *jugador);

// Obtiene el nombre del equipo.
// Pre: El equipo fue creado.
// Post: Devuelve el nombre del equipo.
char* equipo_nombre(equipo_t *equipo);

// Destruye el equipo.
// Pre: El equipo fue creado.
// Post: El equipo es destruido.
void equipo_destruir(equipo_t *equipo);

#endif // EQUIPO_H
