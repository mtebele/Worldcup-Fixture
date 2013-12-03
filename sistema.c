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
};

/******************************************************************
 *                		FUNCIONES AUXILIARES
 ******************************************************************/

int cmp_goles(const void *s1, const void *s2)
{
	jugador_t* jug1 = (jugador_t*)s1;
	jugador_t* jug2 = (jugador_t*)s2;
	if (jugador_goles(jug1) < jugador_goles(jug2)) return -1;
	if (jugador_goles(jug1) > jugador_goles(jug2)) return 1;
	return 0;
}

void listar_jugadores_dorsal(lista_t *lista, jugador_t **plantel)
{
	for (int i = 0; i < MAX_JUG; i++) {
		jugador_t* jugador = plantel[i];
		char buf_dorsal[2];
		char buf_goles[2];
		char **datos = malloc(sizeof(char*) * 3);
		datos[0] = jugador_nombre(jugador);
		snprintf(buf_dorsal, 3, "%d", jugador_dorsal(jugador));
		datos[1] = strdup(buf_dorsal);
		snprintf(buf_goles, 10, "%d", jugador_goles(jugador));
		datos[2] = strdup(buf_goles);
		
		lista_insertar_ultimo(lista, datos);
	}
}

void listar_jugadores_nombre(sistema_t *sistema, lista_t *lista, equipo_t *equipo)
{
	abb_t* abb_jugadores = equipo_plantel_nombre(equipo);
	abb_iter_t* abb_iter = abb_iter_in_crear(abb_jugadores);
	
	while (!abb_iter_in_al_final(abb_iter)) {
		const char *actual = abb_iter_in_ver_actual(abb_iter);
		jugador_t* jugador = hash_obtener(sistema->jugadores, actual);
		char buf_dorsal[2];
		char buf_goles[2];
		char **datos = malloc(sizeof(char*) * 3);
		datos[0] = jugador_nombre(jugador);
		snprintf(buf_dorsal, 3, "%d", jugador_dorsal(jugador));
		datos[1] = strdup(buf_dorsal);
		snprintf(buf_goles, 10, "%d", jugador_goles(jugador));
		datos[2] = strdup(buf_goles);
		lista_insertar_ultimo(lista, datos);
		abb_iter_in_avanzar(abb_iter);
	}

	abb_iter_in_destruir(abb_iter);
}

 /******************************************************************
 *                IMPLEMENTACION DE LAS PRIMITIVAS
 ******************************************************************/
 
// Crea un sistema.
// Post: devuelve un nuevo sistema vacío.
sistema_t* sistema_crear(sistema_comparar_clave_t cmp)
{
	sistema_t* sistema = malloc(sizeof(sistema_t));
	if (sistema == NULL) return NULL;
	sistema->jugadores = hash_crear(jugador_destruir);
	if (!sistema->jugadores) {
		free(sistema);
		return NULL;
	}
	sistema->equipos = hash_crear(equipo_destruir);
	if (!sistema->equipos) {
		free(sistema->jugadores);
		free(sistema);
		return NULL;
	}
	sistema->goleadores = heap_crear(cmp_goles);
	if (!sistema->goleadores) {
		free(sistema->jugadores);
		free(sistema->equipos);
		free(sistema);
		return NULL;
	}
	sistema->fixture = NULL;
	sistema->comparar = cmp;
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
	
	/*Simulo el partido*/
	partido_t *partido = fixture_partido(sistema->fixture, idr, cantidad);

	/*Obtengo los equipos involucrados*/
	if (!partido_hay_clasificados(partido))
		return RESULTADO_NOEXISTE;

	char *nombre_local = partido_nombre_local(partido);
	char *nombre_visitante = partido_nombre_visitante(partido);

	bool ok = partido_jugar(partido, goles_local, goles_visitante);
	if (!ok) return RESULTADO_YAEXISTE;

	equipo_t* local = hash_obtener(sistema->equipos, nombre_local);
	equipo_t* visitante = hash_obtener(sistema->equipos, nombre_visitante);
	
	/*Actualizo info de jugadores de los equipos involucrados*/
	int i = 0;
	int offset_local = 3;
	while (i < goles_local) {
		int dorsal = atoi(vec_parametros[i+offset_local]);
		equipo_agregar_gol(local, dorsal);
		i++;
	}
	
	int offset_visita = i + 3;
	i = 0;
	while (i < goles_visitante) {
		int dorsal = atoi(vec_parametros[i+offset_visita]);
		equipo_agregar_gol(visitante, dorsal);
		i++;
	}

	/*Actualizo los goleadores del torneo*/	
	heap_heapify(sistema->goleadores);

	/*Clasifico al ganador a la próxima ronda*/
	if (fixture_final(sistema->fixture, idr, cantidad-1))
		return OK;
	
	char* ganador = partido_ganador(partido);
	partido_t* partido_siguiente = fixture_clasificar_equipo(sistema->fixture, idr, cantidad);
	
	bool clasifico;
	int pos = obtener_instancia(idr, fixture_tamanio(sistema->fixture));
	if (pos % 2 == 0)
		clasifico = partido_agregar_local(partido_siguiente, ganador);
	else
		clasifico = partido_agregar_visitante(partido_siguiente, ganador);

	if (!clasifico)
		return ERROR;

	return OK;
}

lista_t* sistema_listar_jugadores(sistema_t* sistema, char* modo, char* nombre)
{
	equipo_t* equipo = hash_obtener(sistema->equipos, nombre);
	if (!equipo) return NULL;

	lista_t* lista = lista_crear();
	if (!lista) {
		free(equipo);
		return NULL;
	}

	jugador_t** plantel = equipo_plantel(equipo);
	upcase(modo);

	if (strcmp(modo, "DORSAL") == 0)
		listar_jugadores_dorsal(lista, plantel);
	else if (strcmp(modo, "NOMBRE") == 0)
		listar_jugadores_nombre(sistema, lista, equipo);
	else
		printf("Modo de consulta inválido\n");

	return lista;
}

char** sistema_listar_goleador(sistema_t* sistema)
{
	jugador_t *jugador = heap_ver_max(sistema->goleadores);
	if (!jugador) return NULL;
	
	char buf_goles[2];
	char** datos = malloc(3 * sizeof(char*));
	datos[0] = jugador_nombre(jugador);
	datos[1] = jugador_equipo(jugador);
	snprintf(buf_goles, 10, "%d", jugador_goles(jugador));
	datos[2] = strdup(buf_goles);
	return datos;
}

char** sistema_goles_jugador(sistema_t* sistema, char* nombre)
{
	jugador_t* jugador = hash_obtener(sistema->jugadores, nombre);
	if (!jugador) return NULL;
	
	char** datos = malloc(3 * sizeof(char*));
	char buf_dorsal[2];
	char buf_goles[2];	
	snprintf(buf_dorsal, 3, "%d", jugador_dorsal(jugador));
	datos[0] = strdup(buf_dorsal);
	datos[1] = jugador_equipo(jugador);
	snprintf(buf_goles, 10, "%d", jugador_goles(jugador));
	datos[2] = strdup(buf_goles);
	return datos;
}

char** sistema_mostrar_resultado(sistema_t* sistema, char* idr)
{
	size_t cantidad = sistema_cantidad_equipos(sistema);
	partido_t *partido = fixture_partido(sistema->fixture, idr, cantidad);
	if (!partido) return NULL;
	
	char** datos = NULL;
	if (partido_jugado(partido)) {
		datos = malloc(4 * sizeof(char*));
		char buf_local[2];
		char buf_visita[2];
		
		datos[0] = partido_nombre_local(partido);
		snprintf(buf_local, 10, "%d", partido_goles_local(partido));
		datos[1] = strdup(buf_local);
		datos[2] = partido_nombre_visitante(partido);
		snprintf(buf_visita, 10, "%d", partido_goles_visitante(partido));
		datos[3] = strdup(buf_visita);
	}		

	return datos;
}

bool sistema_agregar_equipo(sistema_t* sistema, char* nombre)
{
	equipo_t* equipo = equipo_crear(nombre);
	if (!equipo) return false;
	return hash_guardar(sistema->equipos, nombre, equipo);
}

bool sistema_agregar_jugador(sistema_t* sistema, int dorsal, char* equipo, char* nombre)
{
	// Crea el jugador
	jugador_t* jugador = jugador_crear(nombre, equipo, dorsal);
	if (!jugador) return false;
	
	// Agrega el jugador al hash (jugadores)
	if (!hash_guardar(sistema->jugadores, nombre, jugador))
		return false;
	
	// Agrega el jugador al heap (goleadores)
	if (!heap_encolar(sistema->goleadores, jugador))
		return false;
	
	// Inscribe el jugador al equipo
	equipo_t* equipo_jug = hash_obtener(sistema->equipos, equipo);
	if (!equipo_inscribir(equipo_jug, jugador))
		return false;

	return true;
}

bool sistema_cargar_fixture(sistema_t* sistema, lista_t* lista)
{
	sistema->fixture = fixture_crear(lista_largo(lista)-1);
	if (!sistema->fixture) return false;	
	return fixture_cargar(sistema->fixture, lista);
}

void sistema_destruir(sistema_t* sistema)
{
	fixture_destruir(sistema->fixture);
	hash_destruir(sistema->jugadores);	
	hash_destruir(sistema->equipos);
	heap_destruir(sistema->goleadores, NULL);
	free(sistema);
	return;
}
