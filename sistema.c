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
	partido_t **fixture;
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

int idrtopos(const char *idr, size_t n)
{
	int instancia = idr[0] - '0';
	if (instancia == 1) return n-1;
	int inicio_instancia = n - 2* instancia;
	int letra = idr[1] - 'a';
	int pos = inicio_instancia + letra;
	return pos;
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
	if (sistema->jugadores == NULL) {
		free(sistema);
		return NULL;
	}
	sistema->equipos = hash_crear(destruir_dato);
	if (sistema->equipos == NULL) {
		free(sistema->jugadores);
		free(sistema);
		return NULL;
	}
	sistema->goleadores = heap_crear(cmp_max);
	if (sistema->goleadores == NULL) {
		free(sistema->jugadores);
		free(sistema->equipos);
		free(sistema);
		return NULL;
	}
	size_t cant_partidos = sistema_cantidad_equipos(sistema) - 1;
	sistema->fixture = malloc(sizeof(partido_t) * cant_partidos);
	if (sistema->fixture == NULL) {
		free(sistema->goleadores);		
		free(sistema->jugadores);
		free(sistema->equipos);
		free(sistema);
		return NULL;
	}
	//falta inicializar el fixture con todos los partidos "vacios"
	sistema->comparar = cmp;
	sistema->destruir_dato = destruir_dato;
	return sistema;
}

//Determina la cantidad de equipos participantes en el torneo
//Pre: El sistema fue creado
//Post: Devuelve la cantidad de equipos particpantes en el torneo
size_t sistema_cantidad_equipos(sistema_t *sistema)
{
	return hash_cantidad(sistema->equipos);
}

resultado_t sistema_agregar_resultado(sistema_t* sistema, char* vec_parametros[])
{
	char* idr = vec_parametros[0];
	int gloc = atoi(vec_parametros[1]);
	int gvis = atoi(vec_parametros[2]);
	int pos = idrtopos(idr, sistema_cantidad_equipos(sistema) - 1);

	/*Falta chequear si los partidos previos fueron jugados*/
	/*Simulo el partido*/
	partido_t *partido = sistema->fixture[pos];
	bool simulado = partido_simular(partido, gloc, gvis);
	if(!simulado) return NONE; //algo, no se si none,

	/*Obtengo los equipos involucrados*/
	char *nombre_local = partido_local(partido);
	char *nombre_visitante = partido_visitante(partido);

	equipo_t* local = hash_obtener(sistema->equipos, nombre_local);
	equipo_t* visitante = hash_obtener(sistema->equipos, nombre_visitante);
	
	/*Actualizo info de jugadores de los equipos involucrados*/
	int i = 3;
	while (i < gloc) {
		int dorsal = vec_parametros[i];
		equipo_agregar_gol(local, dorsal);
		i++;
	}

	while (i < gvis) {
		int dorsal = vec_parametros[i];
		equipo_agregar_gol(visitante, dorsal);
		i++;
	}

	/*Actualizo los goleadores del torneo*/	
	heap_heapify(sistema->goleadores);

	/*Clasifico al ganador a la próxima ronda*/
	if (pos == sistema_cantidad_equipos(sistema))
		return OK;
	int posprox = pos;	
	if (posprox % 2 == 1)
		posprox--;
	int offset = sistema_cantidad_equipos(sistema) / 2 - posprox / 2;
	posprox += offset;
	
	char* ganador = partido_ganador(partido);
	partido_t *prox_ronda = sistema->fixture[posprox];
	bool clasifico;
	if(pos % 2 == 0)
		clasifico = partido_agregar_local(prox_ronda, ganador);
	else
		clasifico = partido_agregar_visitante(prox_ronda, ganador);
	if(clasifico)
		return OK;
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
	size_t pos = idrtopos(idr, sistema_cantidad_equipos(sistema->equipos) - 1);
	partido_t *partido = sistema->fixture[pos];
	if (!partido) return NULL;

	char linea[BUFSIZ];
	if(partido_jugado(partido))
	{
		char *local = partido_local(partido);
		int goles_loc = partido_goles_local(partido);
		char *visita = partido_visitante(partido);
		int goles_vis = partido_goles_visitante(partido);
		sprintf(linea, "%s,%d: %s Goles: %d", local, goles_loc, visita, goles_vis);
	} else sprintf(linea, "Error : el resultado con id %s no existe", idr);

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
