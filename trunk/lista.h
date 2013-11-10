#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h>

/*
 * NOMBRE: Matias Tebele
 * PADRON: 95372
 */

/******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 ******************************************************************/

/* La cola est� planteada como una cola de punteros gen�ricos. */

typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;

/******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 ******************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vac�a.
lista_t* lista_crear();

// Destruye la lista. Si se recibe la funci�n destruir_dato por par�metro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una funci�n capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void*));

// Devuelve verdadero o falso, seg�n si la lista tiene o no elementos enlazados.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento al principio de la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agreg� un nuevo elemento a la lista, dato se encuentra al principio
// de la lista.
bool lista_insertar_primero(lista_t *lista, void* dato);

// Agrega un nuevo elemento al final de la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agreg� un nuevo elemento a la lista, dato se encuentra al final
// de la lista.
bool lista_insertar_ultimo(lista_t *lista, void* dato);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si est� vac�a devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvi� el primer elemento de la lista, cuando no est� vac�a.
void* lista_ver_primero(const lista_t *lista);

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su valor, si est� vac�a, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvi� el valor del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vac�a.
void* lista_borrar_primero(lista_t *lista);

// Devuelve el tama�o de la lista
// Pre: la lista fue creada.
size_t lista_largo(const lista_t *lista);

/******************************************************************
 *                    PRIMITIVAS DE ITERACION
 ******************************************************************/

// Crea un interador.
// Pre: la lista fue creada.
// Post: se devolvi� un nuevo iterador enlazado al primer elemento de la lista.
lista_iter_t *lista_iter_crear(const lista_t *lista);

// Avanza el iterador al siguiente nodo. Si el iterador no esta en el final,
// el nodo actual pasa a ser el anterior, y el siguiente el actual y devuelve
// true. Caso contrario, devuelve false.
// Pre: el iterador fue creado
// Post: el iterador apunt� al proximo elemento.
bool lista_iter_avanzar(lista_iter_t *iter);

// Obtiene el valor del elemento al que apunta el iterador.
// Pre: el iterador fue creado.
// Post: se devolvi� el elemento al que apunta el iterador.
void* lista_iter_ver_actual(const lista_iter_t *iter);

// Verifica si el iterador llego al final de la lista. Devuelve true si est�
// en la posicion siguiente al �ltimo elemento, false en caso contrario.
// Pre: el iterador fue creado.
// Post: se verific� la posici�n del iterador.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador fue creado.
// Post: se elimin� el iterador.
void lista_iter_destruir(lista_iter_t *iter);

// Agrega un nuevo elemento a la lista en la posici�n en que apunta el iterador,
// entre el que apunta y su antecesor, y se incrementa en uno la cantidad de elementos.
// Devuelve falso en caso de error.
// Pre: la lista y el iterador fueron creados.
// Post: se agreg� un nuevo elemento a la lista, dato se encuentra en la posici�n
// en que apunta el iterador, y el iterador apunt� al nuevo dato.
bool lista_insertar(lista_t *lista, lista_iter_t *iter, void* dato);

// Saca el N elemento de la lista, donde N es la posici�n en que apunta el iterador.
// Si la lista tiene elementos y el iterador no est� al final, se quita el
// elemento de la lista, se devuelve su valor y se decrementa en uno la cantidad.
// Si la lista est� vac�a o el iterador apuntando al final de la misma, devuelve NULL.
// Pre: la lista y el iterador fueron creados.
// Post: se devolvi� el valor del elemento anterior. Si el iterador apuntaba al �ltimo
// elemento, ahora apunta al nuevo �ltimo. Si el iterador apuntaba al primer elemento,
// ahora apunta al siguiente.
void* lista_borrar(lista_t *lista, lista_iter_t *iter);

// Recorre la lista elemento por elemento mientras que la funci�n "visitar" devuelve false,
// o bien se llegue al final de la lista. Si la lista no tiene elementos, devuelve false.
// La funci�n de callback "visitar" recibe el dato y un puntero extra, y devuelve true si se
// debe seguir iterando, false en caso contrario.
// Pre: la lista fue creada.
// Post: se recorri� la lista llamando a la funci�n "visitar" por cada elemento recorrido.
void lista_iterar(lista_t *lista, bool (*visitar)(void* dato, void* extra), void* extra);

#endif // LISTA_H
