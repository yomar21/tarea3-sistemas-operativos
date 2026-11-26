#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    ASIGNACION_CONTIGUA = 1,
    ASIGNACION_ENLAZADA = 2,
    ASIGNACION_INDEXADA = 3
} MecanismoAsignacion;

typedef struct {
    uint64_t tamano_disco_mb;
    uint64_t tamano_bloque_kb;
    MecanismoAsignacion mecanismo;
    uint64_t tamano_min_archivo_kb;
    uint64_t tamano_max_archivo_kb;
} Configuracion;

/*
 * Solicita la configuracion por stdin y repite cada campo hasta validarlo.
 * Los tamanos son enteros positivos y se usa 1 MB = 1024 KB.
 * El bloque y los archivos deben caber en el disco; minimo <= maximo.
 * No se exige que los tamanos sean multiplos del bloque.
 * Devuelve false si recibe NULL, se termina la entrada o falla su lectura.
 * Solo modifica *configuracion cuando todos los campos son validos.
 */
bool configuracion_leer(Configuracion *configuracion);

#endif
