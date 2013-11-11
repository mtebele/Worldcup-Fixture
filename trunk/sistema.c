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
	hash_t* equipos;
	heap_t* goleadores;
	sistema_comparar_clave_t comparar;
	sistema_destruir_dato_t destruir_dato;
};

/******************************************************************
 *                		FUNCIONES AUXILIARES
 ******************************************************************/

int cmp_max(const void *s1, const void *s2)
{
	if (*(int*)s1 < *(int*)s2) return -1;
	if (*(int*)s1 > *(int*)s2) return 1;
	return 0;
}
 
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
	sistema->equipos = hash_crear(destruir_dato);
	if (sistema->equipos == NULL) {
		free(sistema->fixture);
		free(sistema->jugadores);
		free(sistema);
		return NULL;
	}
	sistema->goleadores = heap_crear(cmp_max);
	if (sistema->goleadores == NULL) {
		free(sistema->fixture);
		free(sistema->jugadores);
		free(sistema->equipos);
		free(sistema);
		return NULL;
	}
	sistema->comparar = cmp;
	sistema->destruir_dato = destruir_dato;
	return sistema;
}

resultado_t sistema_agregar_resultado(sistema_t* sistema, char* vec_parametros[])
{
	char* idr = vec_parametros[0];
	int gloc = atoi(vec_parametros[1]);
	int gvis = atoi(vec_parametros[2]);
	for (int i = 0; i < gloc; i++) {
		// GOLES LOCAL
	}
	for (int i = 0; i < gvis; i++) {
		// GOLES VISITANTE
	}
	
	// TODO
	return NONE;
}

// FALTA ORDENAR POR NOMBRE
lista_t* sistema_listar_jugadores(sistema_t* sistema, char* vec_parametros[])
{
	/*char* orden = vec_parametros[0];
	char* nombre = vec_parametros[1];
	
	equipo_t* equipo = hash_obtener(sistema->equipos, nombre);
	
	if (equipo == NULL)	return NULL;
	
	lista_t* lista = lista_crear();	
	jugador** plantel = equipo->plantel;
	
	for (int i = 0; i < MAX_JUG; i++) {
		
	}*/
	
	
	
	
	
	return NULL;
}

char **sistema_listar_goleador(sistema_t* sistema)
{
	char *datos[3];
	jugador_t *jug = heap_ver_max(sistema->goleadores);
	datos[0] = jugador_nombre(jug);
	datos[1] = jugador_equipo(jug);
	datos[2] = jugador_dorsal(jug);
	return datos;
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
