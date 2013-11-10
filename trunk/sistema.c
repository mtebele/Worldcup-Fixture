#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "sistema.h"

/******************************************************************
 *                IMPLEMENTACION DE LOS TIPOS DE DATOS
 ******************************************************************/

struct sistema
{
	abb_t* fixture;
	hash_t* jugadores;
	sistema_comparar_clave_t comparar;
	sistema_destruir_dato_t destruir_dato;
};

/******************************************************************
 *                IMPLEMENTACION DE LAS PRIMITIVAS
 ******************************************************************/
 
// Crea un sistema.
// Post: devuelve un nuevo sistema vacío.
sistema_t* sistema_crear(sistema_comparar_clave_t cmp, sistema_destruir_dato_t destruir_dato)
{
	sistema_t* sistema = malloc(sizeof(sistema_t));
	if (sistema == NULL) return NULL;
	sistema->fixture = abb_crear(cmp, destruir_dato);
	if (sistema->fixture == NULL) {
		free(sistema);
		return NULL;
	}
	sistema->jugadores = hash_crear(destruir_dato);
	if (sistema->jugadores == NULL) {
		free(sistema->fixture);
		free(sistema);
		return NULL;
	}
	sistema->comparar = cmp;
	sistema->destruir_dato = destruir_dato;
	return sistema;
}

resultado_t sistema_agregar_resultado(sistema_t* sistema, char* vec_parametros[])
{
	return NONE;
}

resultado_t sistema_listar_jugadores(sistema_t* sistema, char* vec_parametros[])
{
	return NONE;
}

resultado_t sistema_listar_goleador(sistema_t* sistema)
{
	return NONE;
}

resultado_t sistema_goles_jugador(sistema_t* sistema, char* nombre)
{
	return NONE;
}

resultado_t sistema_mostrar_resultado(sistema_t* sistema, char* idr)
{
	return NONE;
}

// Destruye la sesión. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la sesión llama a destruir_dato.
// Pre: el sistema fue creado. destruir_dato es una función capaz de destruir
// los datos de la sesión, o NULL en caso de que no se la utilice.
// Post: se destruye la sesión y sus datos.
void sistema_destruir(sistema_t* sistema)
{
	return;
}
