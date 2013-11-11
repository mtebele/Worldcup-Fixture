#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "sistema.h"

#define MAX_JUG 23

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

// Clona una cadena con memoria dinámica
char *strdup(const char *s)
{
    char *n = malloc(strlen(s) + 1);
    if(n == NULL) return NULL;
    strcpy(n, s);
    return n;
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

// OPTIMIZAR ESTO
lista_t* sistema_listar_jugadores(sistema_t* sistema, char* vec_parametros[])
{
	char* orden = vec_parametros[0];
	char* nombre = vec_parametros[1];
	
	equipo_t* equipo = hash_obtener(sistema->equipos, nombre);
	if (equipo == NULL)	return NULL;
	
	lista_t* lista = lista_crear();
	if (lista == NULL) return NULL;
	
	jugador_t** plantel = equipo_plantel(equipo);
	
	if (sistema->comparar(orden, "dorsal") == 0) {
		for (int i = 0; i < MAX_JUG; i++) {
			jugador_t* jugador = plantel[i];
			char* datos[3];
			datos[0] = jugador_nombre(jugador);
			sprintf(datos[1], "%d", jugador_dorsal(jugador));
			sprintf(datos[2], "%d", jugador_goles(jugador));
			lista_insertar_ultimo(lista, datos);
		}
	}
	else {
		abb_t* abb_jugadores = abb_crear(sistema->comparar, sistema->destruir_dato);
		for (int i = 0; i < MAX_JUG; i++) {
			jugador_t* jugador = plantel[i];
			char* nombre = jugador_nombre(jugador);			
			abb_guardar(abb_jugadores, nombre, jugador);
		}
		abb_iter_t* abb_iter = abb_iter_in_crear(abb_jugadores);
		while (!abb_iter_in_al_final(abb_iter)) {
			// abb_obtener o usar el hash?
			jugador_t* jugador = hash_obtener(sistema->jugadores, nombre);
			char* datos[3];
			datos[0] = jugador_nombre(jugador);
			sprintf(datos[1], "%d", jugador_dorsal(jugador));
			sprintf(datos[2], "%d", jugador_goles(jugador));
			lista_insertar_ultimo(lista, datos);
		}
	}
	
	return lista;
}

char** sistema_listar_goleador(sistema_t* sistema)
{
	jugador_t *jugador = heap_ver_max(sistema->goleadores);
	if (!jugador) return NULL;

	char *datos[3];
	datos[0] = jugador_nombre(jugador);
	datos[1] = jugador_equipo(jugador);
	sprintf(datos[2], "%d", jugador_dorsal(jugador));
	return datos;
}

char** sistema_goles_jugador(sistema_t* sistema, char* nombre)
{
	jugador_t* jugador = hash_obtener(sistema->jugadores, nombre);
	if (jugador == NULL) return NULL;
	
	char *datos[3];
	sprintf(datos[0], "%d", jugador_dorsal(jugador));
	datos[1] = jugador_equipo(jugador);
	sprintf(datos[2], "%d", jugador_goles(jugador));
	return datos;
}

char* sistema_mostrar_resultado(sistema_t* sistema, char* idr)
{
	partido_t *partido = abb_obtener(sistema->fixture, idr);
	if (!partido) return NULL;

	char linea[BUFSIZ];
	char *eq_loc = partido_eq_loc(partido);
	int goles_loc = partido_goles_loc(partido);
	char *eq_vis = partido_eq_vis(partido);
	int goles_vis = partido_goles_vis(partido);
	sprintf(linea, "%s,%d: %s Goles: %d", eq_loc, goles_loc, eq_vis, goles_vis);
	return linea;
}

bool sistema_agregar_equipo(sistema_t* sistema, char* nombre)
{
	equipo_t* equipo = equipo_crear(nombre);
	if (!equipo) return NULL;
	
	//GUARDAR EN FIXTURE
	return hash_guardar(sistema->equipos, nombre, equipo);	
}

bool sistema_agregar_jugador(sistema_t* sistema, int dorsal, char* equipo, char* nombre)
{
	jugador_t* jugador = jugador_crear(nombre, equipo, dorsal);
	if (!jugador) return NULL;
	
	return hash_guardar(sistema->jugadores, nombre, jugador);	
}

// Destruye el sistema.
// Pre: El sistema fue creado.
// Post: El sistema es destruido.
void sistema_destruir(sistema_t* sistema)
{
	abb_destruir(sistema->fixture);	
	hash_destruir(sistema->jugadores);	
	hash_destruir(sistema->equipos);	//destruyo 2 veces??
	heap_destruir(sistema->goleadores, NULL);	//destruí todo antes!
	return;
}
