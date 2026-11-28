#include "disco.h"

#include <stdlib.h>

#define KB_POR_MB UINT64_C(1024)

static bool indice_valido(const Disco *disco, size_t indice) {
    return disco != NULL && disco->ocupados != NULL &&
           indice < disco->cantidad_bloques;
}

bool disco_inicializar(Disco *disco, uint64_t tamano_disco_mb,
                       uint64_t tamano_bloque_kb) {
    if (disco == NULL || disco->ocupados != NULL || tamano_disco_mb == 0 ||
        tamano_disco_mb > UINT64_MAX / KB_POR_MB || tamano_bloque_kb == 0) {
        return false;
    }

    uint64_t capacidad_kb = tamano_disco_mb * KB_POR_MB;
    uint64_t cantidad = capacidad_kb / tamano_bloque_kb;
    if (cantidad == 0 || cantidad > SIZE_MAX / sizeof(bool)) {
        return false;
    }

    /* Se almacena el estado de los bloques, no su contenido simulado. */
    bool *ocupados = calloc((size_t)cantidad, sizeof *ocupados);
    if (ocupados == NULL) {
        return false;
    }

    *disco = (Disco){
        .capacidad_kb = capacidad_kb,
        .tamano_bloque_kb = tamano_bloque_kb,
        .sobrante_kb = capacidad_kb % tamano_bloque_kb,
        .cantidad_bloques = (size_t)cantidad,
        .bloques_libres = (size_t)cantidad,
        .ocupados = ocupados
    };
    return true;
}

void disco_destruir(Disco *disco) {
    if (disco != NULL) {
        free(disco->ocupados);
        *disco = (Disco){0};
    }
}

bool disco_bloque_libre(const Disco *disco, size_t indice) {
    return indice_valido(disco, indice) && !disco->ocupados[indice];
}

bool disco_ocupar_bloque(Disco *disco, size_t indice) {
    if (!disco_bloque_libre(disco, indice)) {
        return false;
    }

    disco->ocupados[indice] = true;
    --disco->bloques_libres;
    return true;
}

bool disco_liberar_bloque(Disco *disco, size_t indice) {
    if (!indice_valido(disco, indice) || !disco->ocupados[indice]) {
        return false;
    }

    disco->ocupados[indice] = false;
    ++disco->bloques_libres;
    return true;
}

uint64_t disco_espacio_libre_kb(const Disco *disco) {
    if (disco == NULL || disco->ocupados == NULL) {
        return 0;
    }
    return (uint64_t)disco->bloques_libres * disco->tamano_bloque_kb;
}
