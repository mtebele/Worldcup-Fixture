#ifndef SISTEMA_H
#define SISTEMA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "equipo.h"
#include "abb.h"
#include "hash.h"

/******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 ******************************************************************/

typedef struct sistema sistema_t;
typedef enum {OK, IDC_REPETIDO, IDC_NOEXISTE, PAD_INSC, PAD_NOINSC, SES_NULA, SES_ENCURSO, CUR_SEL, ACC_DES,
		      REGULAR, CONDIC, SES_NOINS, SES_REGULAR, SES_CONDIC, INS_NOEXISTE, ERROR, NONE} resultado_t;
		      
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

resultado_t sistema_listar_jugadores(sistema_t* sistema, char* vec_parametros[]);

resultado_t sistema_listar_goleador(sistema_t* sistema);

resultado_t sistema_goles_jugador(sistema_t* sistema, char* nombre);

resultado_t sistema_mostrar_resultado(sistema_t* sistema, char* idr);

void sistema_destruir(sistema_t* sistema);

#endif // SISTEMA_H
