#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "mensajes.h"
#include "sistema.h"

#define CANT_COMANDOS 12
#define MAX_PARAMETROS 50
#define comparar(a,b) strcmp(a,b)==0

/****************************************
		DEFINICION DE FUNCIONES
****************************************/

void ejecutar_servicio(sistema_t *sistema, char *comando, char **parametros, int cantidad)
{
	// Compara el comando y ejecuta la acción correspondiente
	if (comparar(comando, "AGREGAR_RESULTADO")) {
		resultado_t res = sistema_agregar_resultado(sistema, parametros);
		char* idr = parametros[0];
		switch (res) {
			case OK: mensaje_ok(); break;
			case RESULTADO_NOEXISTE: mensaje_resultado_no_existente(idr); break;
			case RESULTADO_YAEXISTE: mensaje_resultado_ya_existente(idr); break;
			default: mensaje_error();
		}
	}
	else if (comparar(comando, "LISTAR_JUGADORES") && cantidad == 1) {
		char* cadena = parametros[0];
		char* modo = strtok(cadena, " ");
		char* nombre = strtok(NULL, "");

		lista_t* lista = sistema_listar_jugadores(sistema, modo, nombre);
		
		if (!lista)	{
			mensaje_equipo_no_inscripto(nombre);
			return;
		}			
		while (!lista_esta_vacia(lista)) {
			char** datos = lista_borrar_primero(lista);
			char* jugador = datos[0];
			int dorsal = atoi(datos[1]);
			int goles = atoi(datos[2]);
			mensaje_listar_jugadores(jugador, dorsal, goles);
			
			for (int i=0; i < 3; i++)				
				free(datos[i]);
			free(datos);
		}
		lista_destruir(lista, NULL);
	}
	else if (comparar(comando, "LISTAR_GOLEADOR")) {
		char** datos = sistema_listar_goleador(sistema);
		char* jugador = datos[0];
		char* equipo = datos[1];
		int goles = atoi(datos[2]);
		mensaje_listar_goleador(jugador, equipo, goles);
		for (int i=0; i < 3; i++)				
				free(datos[i]);
		free(datos);		
	}
	else if (comparar(comando, "GOLES_JUGADOR")) {
		char* jugador = parametros[0];
		char** datos = sistema_goles_jugador(sistema, jugador);
		
		if (!datos)
			mensaje_jugador_no_inscripto(jugador);
		else {
			int dorsal = atoi(datos[0]);
			char* equipo = datos[1];
			int goles = atoi(datos[2]);
			mensaje_goles_jugador(jugador, dorsal, equipo, goles);
			for (int i=0; i < 3; i++)				
					free(datos[i]);
			free(datos);				
		}
	}
	else if (comparar(comando, "MOSTRAR_RESULTADO")){
			char** datos = sistema_mostrar_resultado(sistema, parametros[0]);
			if (datos) {
				char* equipo1 = datos[0];
				int goles1 = atoi(datos[1]);
				char* equipo2 = datos[2];
				int goles2 = atoi(datos[3]);
				
				mensaje_mostrar_resultado(equipo1, goles1, equipo2, goles2);
				free(datos[1]);
				free(datos[3]);
				free(datos);
			}
			else
				mensaje_resultado_no_existente(parametros[0]);
		}
	else if (!comparar(comando, ""))
		mensaje_comando_invalido();
}

void iniciar(sistema_t* sistema, lista_t* lista_equipos)
{
	sistema_cargar_fixture(sistema, lista_equipos);
	char instruccion[BUFSIZ];
	
	while (fgets(instruccion, BUFSIZ, stdin)) {	
		char* vec_parametros[MAX_PARAMETROS];
		char* cadena = strtok(instruccion, " ,");
		char* comando = strdup(trim(cadena));
		int cantidad = 0;
		
		while (((cadena = strtok(NULL, ",\n")) != NULL)) {
			vec_parametros[cantidad] = strdup(trim(cadena));
			cantidad++;
		}
		
		upcase(comando);		
		ejecutar_servicio(sistema, comando, vec_parametros, cantidad);
		
		for (int i = 0; i < cantidad; i++)
			free(vec_parametros[i]);	
	
		free(comando);	
	}	
	
	lista_destruir(lista_equipos,NULL);
	sistema_destruir(sistema);
}

lista_t* cargar_archivo(sistema_t *sistema, char *nombrearch, lista_t *lista_equipos)
{
	FILE *archivo;
	char linea[BUFSIZ];
	char* equipo;

	archivo = fopen(nombrearch, "r");
	lista_t *equipos = lista_crear();
	if (!archivo)
		puts("Error de apertura del archivo.");
	else {
 		int i = 0;
		while (feof(archivo) == 0) {
			int dorsal = i % 24;
			
			if (fgets(linea, BUFSIZ, archivo)) {
				trim(linea);
				if (dorsal == 0) {
					// Equipo
					equipo = strdup(trim(linea));
					sistema_agregar_equipo(sistema, equipo);
					lista_insertar_ultimo(lista_equipos, equipo);
					lista_insertar_ultimo(equipos, equipo);
				}
				else {
					// Jugador
					char* jugador = strdup(trim(linea));
					sistema_agregar_jugador(sistema, dorsal, equipo, jugador);
					free(jugador);
				}
			}
			i++;
//			if(i % 24 == 0 && i != 0) free(equipo);
		}
	}
//	free(equipo);
	fclose(archivo);
	return equipos;
}

/* Programa principal. */ 
int main(int argc, char *argv[])
{
	// Creo el sistema
	sistema_t* sistema = sistema_crear(strcmp);
	lista_t* lista_equipos = lista_crear();
 	
	if (argc > 1) {
		lista_t *lis = cargar_archivo(sistema, argv[1], lista_equipos);
		iniciar(sistema, lista_equipos);
		lista_destruir(lis, free);
	}
	else
		printf("Uso: TP2 <archivo>\n");
	
	return 0;
}
