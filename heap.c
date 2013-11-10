#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "heap.h"

#define TAM_INICIAL 511

/*
 * NOMBRE: Matias Tebele | PADRON: 95372
 * NOMBRE: Lucas Perea | PADRON: 95368
 */

/******************************************************************
 *                IMPLEMENTACION DE LOS TIPOS DE DATOS
 ******************************************************************/

struct heap
{	
	cmp_func_t cmp;
	size_t cantidad;	
	size_t tamanio;
	void **datos;
};

/******************************************************************
 *                		FUNCIONES AUXILIARES
 ******************************************************************/

void swap(void** x, void** y)
{
	void* aux = *x;
	*x = *y;
	*y = aux;
}

void upheap(heap_t *heap)
{
	size_t pos = heap_cantidad(heap);
	while (pos > 0) {
		size_t pos_padre = (pos-1) / 2;
		int comparar = heap->cmp(heap->datos[pos], heap->datos[pos_padre]);
		if (comparar > 0) {
			swap(&heap->datos[pos], &heap->datos[pos_padre]);
			pos = pos_padre;
		}
		else return;
	}	
}

void downheap(void **datos, size_t inicio, size_t fin, cmp_func_t cmp)
{
	size_t aux = inicio;	
	size_t hijoIzq = aux*2 + 1;
	size_t hijoDer = aux*2 + 2;
	int comparar;
	if (hijoIzq == fin) {
		comparar = cmp(datos[aux], datos[hijoIzq]);
		if (comparar < 0)
			swap(&datos[aux], &datos[hijoIzq]);
	}
	else if (hijoIzq < fin) {
		comparar = cmp(datos[hijoIzq], datos[hijoDer]);
		aux = (comparar > 0) ? hijoIzq : hijoDer;
		comparar = cmp(datos[inicio], datos[aux]);
		if (comparar < 0) {
			swap(&datos[inicio], &datos[aux]);
			downheap(datos, aux, fin, cmp);
		}
	}
}

bool heap_redimensionar(heap_t* heap, size_t tam_nuevo)
{
	void** datos_nuevo = realloc(heap->datos, tam_nuevo * sizeof(void*));
	if (!datos_nuevo) return false;
	heap->datos = datos_nuevo;
	heap->tamanio = tam_nuevo;
	return true;
}

void heapify(void **datos, size_t cant, cmp_func_t cmp)
{
	size_t mitad = cant / 2;
	for (int i = mitad; i >= 0; i--)
		downheap(datos, i, cant, cmp);
}

/******************************************************************
 *                IMPLEMENTACION DE LAS PRIMITIVAS
 ******************************************************************/

void heap_sort(void *elementos[], size_t largo, cmp_func_t cmp)
{
	heapify(elementos, largo-1, cmp);
	for (int i = largo-1; i > 0; i--) {
		swap(&elementos[0], &elementos[i]);
		downheap(elementos, 0, i-1, cmp);
	}
}

heap_t *heap_crear(cmp_func_t cmp)
{
	heap_t *heap = malloc(sizeof(heap_t));
	if (!heap) return NULL;
	heap->datos = malloc(sizeof(void*) * TAM_INICIAL);
	if (!heap->datos) {
		free(heap);
		return NULL;
	}
	heap->cantidad = 0;
	heap->tamanio = TAM_INICIAL;
	heap->cmp = cmp;
	
	return heap;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e))
{
	if (destruir_elemento)
		for (int i=0; i < heap->cantidad; i++)
			destruir_elemento(heap->datos[i]);
	free(heap->datos);
	free(heap);
}

size_t heap_cantidad(const heap_t *heap)
{
	return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap)
{
	if (!heap) return false;
	return (heap->cantidad == 0);
}

bool heap_encolar(heap_t *heap, void *elem)
{
	if (heap->cantidad == heap->tamanio) {
		size_t tam_nuevo = 2 * heap->tamanio + 1;
		if (!heap_redimensionar(heap, tam_nuevo))
			return false;
	}	
	size_t pos = heap->cantidad;
	heap->datos[pos] = elem;
	upheap(heap);
	heap->cantidad++;
	return true;
}

void *heap_ver_max(const heap_t *heap)
{
	if (!heap || heap_esta_vacio(heap)) return NULL;
	return heap->datos[0];
}

void *heap_desencolar(heap_t *heap)
{
	if (!heap || heap_esta_vacio(heap)) return NULL;
	void* dato = heap_ver_max(heap);
	void* ult = heap->datos[heap->cantidad - 1];
	heap->datos[0] = ult;
	downheap(heap->datos, 0, heap_cantidad(heap)-1, heap->cmp);
	heap->cantidad--;
	
	if (heap->cantidad < heap->tamanio/4 && heap->tamanio/2 >= TAM_INICIAL)	
		heap_redimensionar(heap, heap->tamanio/2);

	return dato;
}

void heap_heapify(heap_t* heap)
{
	heapify(heap->datos, heap->cantidad, heap->cmp);
}
