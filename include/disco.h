#ifndef DISCO_H
#define DISCO_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Los campos se consultan desde otros modulos, pero solo disco.c los modifica. */
typedef struct {
    uint64_t capacidad_kb;
    uint64_t tamano_bloque_kb;
    uint64_t sobrante_kb;
    size_t cantidad_bloques;
    size_t bloques_libres;
    bool *ocupados;
} Disco;

/*
 * Requiere un Disco inicializado con {0} o previamente destruido.
 * Usa 1 MB = 1024 KB y solo crea bloques completos, inicialmente libres.
 * Devuelve false ante parametros invalidos, desbordamiento, falta de memoria
 * o un disco ya inicializado. En esos casos no modifica el disco.
 */
bool disco_inicializar(Disco *disco, uint64_t tamano_disco_mb,
                       uint64_t tamano_bloque_kb);

/* Libera el registro y deja los campos en cero. Acepta NULL y llamadas repetidas. */
void disco_destruir(Disco *disco);

/* Los indices empiezan en cero. Un indice invalido nunca se considera libre. */
bool disco_bloque_libre(const Disco *disco, size_t indice);

/* Rechazan indices invalidos y operaciones repetidas sin alterar los contadores. */
bool disco_ocupar_bloque(Disco *disco, size_t indice);
bool disco_liberar_bloque(Disco *disco, size_t indice);

/* Capacidad asignable libre, sin incluir el sobrante que no forma un bloque. */
uint64_t disco_espacio_libre_kb(const Disco *disco);

#endif
