#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "mensajes.h"
#include "sistema.h"

#define CANT_COMANDOS 12
#define MAX_PAL 5

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

/* Programa principal. */ 
int main(int argc, char *argv[])
{
	FILE *archivo;
	char linea[BUFSIZ];
 	
	if (argc > 1) {
		archivo = fopen(argv[1],"r");
 
		if (!archivo) { 
			printf("\nError de apertura del archivo. \n\n");
		}
		else {
	 		printf("\nEl contenido del archivo de prueba es \n\n");
	 		int i = 0;
			while (feof(archivo) == 0) {
				int dorsal = i % 24;
				if (fgets(linea,BUFSIZ,archivo)) {
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
	return 0;
}

/*int main(void){
	char cad[BUFSIZ];
	while(fgets(cad,BUFSIZ,stdin)){	
		char* linea[MAX_PAL];	
		char* cadena = strtok(cad," \n");
		char* com = strdup(cadena);
		linea[0] = com;	
		int cant_pal = 1;	
		while(((cadena = strtok(NULL,",\n")) != NULL) && cant_pal<MAX_PAL+1){
			linea[cant_pal] = strdup(cadena);
			cant_pal++;
		}		
		funcion_llamar(sistema,linea,cant_pal);
		for(int i=0; i < cant_pal; i++)
			free(linea[i]);	
	}
	
	return 0;
	
}*/
