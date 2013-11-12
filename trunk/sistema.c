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
	fixture_t *fixture;
	hash_t *jugadores;
	hash_t *equipos;
	heap_t *goleadores;
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
	sistema->jugadores = hash_crear(destruir_dato);
	if (!sistema->jugadores) {
		free(sistema);
		return NULL;
	}
	sistema->equipos = hash_crear(destruir_dato);
	if (!sistema->equipos) {
		free(sistema->jugadores);
		free(sistema);
		return NULL;
	}
	sistema->goleadores = heap_crear(cmp_max);
	if (!sistema->goleadores) {
		free(sistema->jugadores);
		free(sistema->equipos);
		free(sistema);
		return NULL;
	}
	sistema->fixture = NULL;
	sistema->comparar = cmp;
	sistema->destruir_dato = destruir_dato;
	return sistema;
}

size_t sistema_cantidad_equipos(sistema_t* sistema)
{
	return hash_cantidad(sistema->equipos);
}

resultado_t sistema_agregar_resultado(sistema_t* sistema, char* vec_parametros[])
{
	char* idr = vec_parametros[0];
	int goles_local = atoi(vec_parametros[1]);
	int goles_visitante = atoi(vec_parametros[2]);
	size_t cantidad = sistema_cantidad_equipos(sistema);
	
printf("cantidad: %d\n", cantidad);
printf("idr: %s | local : %d | visitante : %d\n", idr, goles_local, goles_visitante);

	/*Falta chequear si los partidos previos fueron jugados*/
	/*Simulo el partido*/
	partido_t *partido = fixture_partido(sistema->fixture, idr, cantidad);
	
printf("%s vs %s\n", partido_local(partido), partido_visitante(partido));

	bool simulado = partido_simular(partido, goles_local, goles_visitante);
	if (!simulado) return NONE; //algo, no se si none,

puts("sigue1");

	/*Obtengo los equipos involucrados*/
	char *nombre_local = partido_local(partido);
	char *nombre_visitante = partido_visitante(partido);

	equipo_t* local = hash_obtener(sistema->equipos, nombre_local);
	equipo_t* visitante = hash_obtener(sistema->equipos, nombre_visitante);
	
puts("sigue2");
	
	/*Actualizo info de jugadores de los equipos involucrados*/
	int i = 3;
	while (i < goles_local) {
		int dorsal = atoi(vec_parametros[i]);
		equipo_agregar_gol(local, dorsal);
		i++;
	}

	while (i < goles_visitante) {
		int dorsal = atoi(vec_parametros[i]);
		equipo_agregar_gol(visitante, dorsal);
		i++;
	}

	/*Actualizo los goleadores del torneo*/	
	heap_heapify(sistema->goleadores);

	/*Clasifico al ganador a la próxima ronda*/
	if (fixture_final(sistema->fixture, idr, cantidad))
		return OK;
	
	char* ganador = partido_ganador(partido);
	partido_t* partido_siguiente = fixture_clasificar_equipo(sistema->fixture, idr, cantidad);
	
	bool clasifico;
	if (strcmp(ganador, nombre_local) == 0)
		clasifico = partido_agregar_local(partido_siguiente, ganador);
	else
		clasifico = partido_agregar_visitante(partido_siguiente, ganador);
		
	if (clasifico)
		return OK;
	return NONE;
}

// OPTIMIZAR ESTO
lista_t* sistema_listar_jugadores(sistema_t* sistema, char* vec_parametros[])
{
	char* orden = vec_parametros[0];
	char* nombre = vec_parametros[1];
	
printf("orden: %s | nombre : %s\n", orden, nombre);
	
	equipo_t* equipo = hash_obtener(sistema->equipos, nombre);
	if (equipo == NULL)	return NULL;

printf("equipo: %s\n", equipo_nombre(equipo));
	
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

	char** datos = malloc(3 * sizeof(char*));
	datos[0] = jugador_nombre(jugador);
	datos[1] = jugador_equipo(jugador);
	sprintf(datos[2], "%d", jugador_dorsal(jugador));
	return datos;
}

char** sistema_goles_jugador(sistema_t* sistema, char* nombre)
{
	jugador_t* jugador = hash_obtener(sistema->jugadores, nombre);
	if (jugador == NULL) return NULL;
	
	char** datos = malloc(3 * sizeof(char*));
	sprintf(datos[0], "%d", jugador_dorsal(jugador));
	datos[1] = jugador_equipo(jugador);
	sprintf(datos[2], "%d", jugador_goles(jugador));
	return datos;
}

char* sistema_mostrar_resultado(sistema_t* sistema, char* idr)
{
	size_t cantidad = sistema_cantidad_equipos(sistema);
	partido_t *partido = fixture_partido(sistema->fixture, idr, cantidad);
	if (!partido) return NULL;

	char linea[BUFSIZ];
	if (partido_jugado(partido)) {
		char *local = partido_local(partido);
		int goles_loc = partido_goles_local(partido);
		char *visita = partido_visitante(partido);
		int goles_vis = partido_goles_visitante(partido);
		sprintf(linea, "%s,%d: %s Goles: %d", local, goles_loc, visita, goles_vis);
	}
	else
		sprintf(linea, "Error : el resultado con id %s no existe", idr);

	return strdup(linea);
}

bool sistema_agregar_equipo(sistema_t* sistema, char* nombre)
{
	equipo_t* equipo = equipo_crear(nombre);
	if (!equipo) return false;
	return hash_guardar(sistema->equipos, nombre, equipo);
}

bool sistema_agregar_jugador(sistema_t* sistema, int dorsal, char* equipo, char* nombre)
{
	jugador_t* jugador = jugador_crear(nombre, equipo, dorsal);
	if (!jugador) return false;	
	return hash_guardar(sistema->jugadores, nombre, jugador);	
}

bool sistema_cargar_fixture(sistema_t* sistema, lista_t* lista)
{
	sistema->fixture = fixture_crear(lista_largo(lista)); //-1?
	if (!sistema->fixture) return false;	
	return fixture_cargar(sistema->fixture, lista);
}

// Destruye el sistema.
// Pre: El sistema fue creado.
// Post: El sistema es destruido.
void sistema_destruir(sistema_t* sistema)
{
	//partido_destruir(sistema->fixture);	// COMO BORRAR TODOS LOS PARTIDOS?
	hash_destruir(sistema->jugadores);	
	hash_destruir(sistema->equipos);	//destruyo 2 veces??
	heap_destruir(sistema->goleadores, NULL);	//destruí todo antes!
	return;
}
