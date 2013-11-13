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

// Clona una cadena con memoria dinámica
char *strdup(const char *s)
{
    char *n = malloc(strlen(s) + 1);
    if(n == NULL) return NULL;
    strcpy(n, s);
    return n;
}

void itoa(char *buf, int base, int d) {
	char *p = buf;
	char *p1, *p2;
	unsigned long ud = d;
	int divisor = 10;

	/* If %d is specified and D is minus, put `-' in the head.  */
	if (base == 'd' && d < 0) {
		*p++ = '-';
		buf++;
		ud = -d;
	} else if (base == 'x') {
		divisor = 16;
	}

	/* Divide UD by DIVISOR until UD == 0.  */
	do {
	int remainder = ud % divisor;

	*p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
	} while (ud /= divisor);

	/* Terminate BUF.  */
	*p = 0;

	/* Reverse BUF.  */
	p1 = buf;
	p2 = p - 1;
	while (p1 < p2) {
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
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

	/*Falta chequear si los partidos previos fueron jugados*/
	/*Simulo el partido*/
	partido_t *partido = fixture_partido(sistema->fixture, idr, cantidad);

	/*Obtengo los equipos involucrados*/
	char *nombre_local = partido_local(partido);
	char *nombre_visitante = partido_visitante(partido);

printf("%s vs %s\n", nombre_local, nombre_visitante);

	bool simulado = partido_simular(partido, goles_local, goles_visitante);
	if (!simulado)
		return NONE; //algo, no se si none,

	equipo_t* local = hash_obtener(sistema->equipos, nombre_local);
	equipo_t* visitante = hash_obtener(sistema->equipos, nombre_visitante);
	
	/*Actualizo info de jugadores de los equipos involucrados*/
	int i = 0;
	int offset_local = 3;
	while (i < goles_local) {
		int dorsal = atoi(vec_parametros[i+offset_local]);
		//printf("dorsal loc: %d\n", dorsal);
		equipo_agregar_gol(local, dorsal);
		i++;
	}
	
	int offset_visita = i + 3;
	i = 0;
	while (i < goles_visitante) {
		int dorsal = atoi(vec_parametros[i+offset_visita]);
		//printf("dorsal vis: %d\n", dorsal);
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
lista_t* sistema_listar_jugadores(sistema_t* sistema, char* modo, char* nombre)
{
	equipo_t* equipo = hash_obtener(sistema->equipos, nombre);
	if (!equipo) 
		return NULL;

	lista_t* lista = lista_crear();
	if (lista == NULL) return NULL;

	jugador_t** plantel = equipo_plantel(equipo);
	if (strcmp(modo, "dorsal") == 0) {
		for (int i = 0; i < MAX_JUG; i++) {
			jugador_t* jugador = plantel[i];
			char buf_dorsal[2];
			char buf_goles[2];
			char **datos = malloc(sizeof(char*) * 3);
			datos[0] = jugador_nombre(jugador);
			itoa(buf_dorsal,10,jugador_dorsal(jugador));
			datos[1] = strdup(buf_dorsal);
			itoa(buf_goles,10,jugador_goles(jugador));
			datos[2] = strdup(buf_goles);
			
			printf("Nombre: %s | Dorsal: %s | Goles: %s\n", datos[0], datos[1], datos[2]);
			
			lista_insertar_ultimo(lista, datos);
		}
	}
	else if (strcmp(modo, "nombre") == 0) {
		abb_t* abb_jugadores = abb_crear(sistema->comparar, jugador_destruir);
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
	else printf("Modo de consulta inválido\n");

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
	itoa(buf_goles, 10, jugador_goles(jugador));
	datos[2] = strdup(buf_goles);
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
		free(local);
		free(visita);
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
	// Crea el jugador
	jugador_t* jugador = jugador_crear(nombre, equipo, dorsal);
	if (!jugador)
		return false;
	
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

// Destruye el sistema.
// Pre: El sistema fue creado.
// Post: El sistema es destruido.
void sistema_destruir(sistema_t* sistema)
{
	fixture_destruir(sistema->fixture);	// COMO BORRAR TODOS LOS PARTIDOS?
	hash_destruir(sistema->jugadores);	
	hash_destruir(sistema->equipos);	//destruyo 2 veces??
	heap_destruir(sistema->goleadores, NULL);	//destruí todo antes!
	free(sistema);
	return;
}
