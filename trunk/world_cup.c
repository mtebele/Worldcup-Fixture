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

// Clona una cadena con memoria dinámica.
char *strdup(const char *s)
{
    char *n = malloc(strlen(s) + 1);
    if (n == NULL) return NULL;
    strcpy(n, s); 
    return n;
}

// Transforma el texto a mayúsculas
void upcase(char* str)
{
	int len = strlen(str);
	for (int i = 0; i < len; i++)
		str[i] = toupper(str[i]);
}

void sistema_iniciar()
{	
	// Creo el sistema
	sistema_t* sistema = sistema_crear();
	char instruccion[BUFSIZ];
	
	while (fgets(instruccion, BUFSIZ, stdin)) {	
		char* vec_parametros[MAX_PARAMETROS];
		char* cadena = strtok(instruccion, " \n");
		char* comando = strdup(cadena);
		vec_parametros[0] = comando;
		int cantidad = 1;
		
		while (((cadena = strtok(NULL, ",\n")) != NULL) && cantidad <= MAX_PARAMETROS) {
			vec_parametros[cantidad] = strdup(cadena);
			cantidad++;
		}
		
		upcase(comando);
		
		// Compara el comando y ejecuta la acción correspondiente
		if (comparar(comando, "AGREGAR_RESULTADO")) {
			sistema_agregar_resultado(sistema, vec_parametros);
		}
		else if (comparar(comando, "LISTAR_JUGADORES")) {
			lista_t* lista = sistema_listar_jugadores(sistema, vec_parametros);
			while (!lista_esta_vacia(lista)) {
				char** datos = lista_borrar_primero(lista);
				mensaje_listar_jugadores(datos[0], datos[1], datos[2]);
			}
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
	FILE *archivo;
	char linea[BUFSIZ];
 	
	if (argc > 1) {
		archivo = fopen(argv[1], "r");
 
		if (!archivo)
			printf("\nError de apertura del archivo. \n\n");
		else {
	 		printf("\nEl contenido del archivo de prueba es \n\n");
	 		int i = 0;
			while (feof(archivo) == 0) {
				int dorsal = i % 24;
				if (fgets(linea, BUFSIZ, archivo)) {
					if (dorsal == 0)
						printf("%s",linea);
					else
						printf("%i %s", dorsal, linea);
				}
				i++;
			}
		}
		fclose(archivo);
	}
	
	sistema_iniciar();
	
	return 0;
}
