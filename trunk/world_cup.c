#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "mensajes.h"
#include "sistema.h"

#define CANT_COMANDOS 12
#define MAX_PARAMETROS 5
#define comparar(a,b) strcmp(a,b)==0

/****************************************
		DEFINICION DE FUNCIONES
****************************************/

// Transforma el texto a mayúsculas
void upcase(char* str)
{
	int len = strlen(str);
	for (int i = 0; i < len; i++)
		str[i] = toupper(str[i]);
}

// Elimina los espacios de adelante y atrás de un texto
char* trim(char* str)
{
	char* start = str;
	while (*start && isspace(*start))
		++start;
	char* i = start;
	char* end = start;
	while (*i)
		if(!isspace(*(i++)))
			end = i;
	*end = 0;
	return start;
}

void sistema_iniciar(sistema_t* sistema, lista_t* lista_equipos)
{
	sistema_cargar_fixture(sistema, lista_equipos);
	char instruccion[BUFSIZ];
	
	while (fgets(instruccion, BUFSIZ, stdin)) {	
		char* vec_parametros[MAX_PARAMETROS];
		char* cadena = strtok(instruccion, " ,");
		char* comando = strdup(trim(cadena));
		int cantidad = 0;
		
		while (((cadena = strtok(NULL, ",\n")) != NULL)) {// && cantidad <= MAX_PARAMETROS) {
			vec_parametros[cantidad] = strdup(trim(cadena));
			cantidad++;
		}
		
		upcase(comando);
		
		// Compara el comando y ejecuta la acción correspondiente
		if (comparar(comando, "AGREGAR_RESULTADO")) {
			sistema_agregar_resultado(sistema, vec_parametros);
		}
		else if (comparar(comando, "LISTAR_JUGADORES")) {
			char* cadena = vec_parametros[0];
			vec_parametros[0] = strtok(cadena, " ");
			vec_parametros[1] = strtok(NULL, " ");
			lista_t* lista = sistema_listar_jugadores(sistema, vec_parametros);
						
			while (!lista_esta_vacia(lista)) {
				char** datos = lista_borrar_primero(lista);
				printf("%s,%s: Goles: %s\n", datos[0], datos[1], datos[2]);
				//mensaje_listar_jugadores(datos[0], atoi(datos[1]), atoi(datos[2])); //TOFIX
			}
			lista_destruir(lista, free);
		}
		else if (comparar(comando, "LISTAR_GOLEADOR")) {
			char** datos = sistema_listar_goleador(sistema);
			mensaje_listar_goleador(datos[0], datos[1], atoi(datos[2]));
		}
		else if (comparar(comando, "GOLES_JUGADOR")) {
			char* jugador = vec_parametros[0];
			char** datos = sistema_goles_jugador(sistema, jugador);
			mensaje_goles_jugador(jugador, atoi(datos[0]), datos[1], atoi(datos[2]));
		}
		else if (comparar(comando, "MOSTRAR_RESULTADO"))
			sistema_mostrar_resultado(sistema, vec_parametros[0]);
		else if (!comparar(comando, ""))
			printf("Comando inválido. Intente nuevamente.\n");		
		
		for (int i = 0; i < cantidad; i++)
			free(vec_parametros[i]);	
	}	
}

/* Programa principal. */ 
int main(int argc, char *argv[])
{
	// Creo el sistema
	sistema_t* sistema = sistema_crear(strcmp, free);
	if (!sistema) puts("NULL");

	lista_t* lista_equipos = lista_crear();
	if (!lista_equipos) puts("LISTA NULL");
	
	FILE *archivo;
	char linea[BUFSIZ];
	char* equipo;
 	
	if (argc > 1) {
		archivo = fopen(argv[1], "r");
 
		if (!archivo)
			puts("Error de apertura del archivo.");
		else {
	 		//puts("El contenido del archivo de prueba es:");
	 		int i = 0;
			while (feof(archivo) == 0) {
				int dorsal = i % 24;
				if (fgets(trim(linea), BUFSIZ, archivo)) {
					if (dorsal == 0) {
						// Equipo
						equipo = strdup(trim(linea));
						//printf("Pais: %s\n", equipo);
						if (!sistema_agregar_equipo(sistema, equipo)) puts("ERROR GUARDAR EQUIPO");
						lista_insertar_ultimo(lista_equipos, equipo);
					}
					else {
						// Jugador
						char* jugador = strdup(trim(linea));
						//printf("%i %s\n", dorsal, jugador);
						if (!sistema_agregar_jugador(sistema, dorsal, equipo, jugador))  puts("ERROR GUARDAR JUGADOR");
					}
				}
				i++;
			}
		}
		free(equipo);
		fclose(archivo);
	}
	
	sistema_iniciar(sistema, lista_equipos);
	
	return 0;
}
