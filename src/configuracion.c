#include "configuracion.h"

#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KB_POR_MB UINT64_C(1024)

static bool leer_entero(const char *mensaje, uint64_t minimo,
                        uint64_t maximo, uint64_t *resultado) {
    char linea[128];

    for (;;) {
        fputs(mensaje, stdout);
        fflush(stdout);

        if (fgets(linea, sizeof linea, stdin) == NULL) {
            return false;
        }

        /* Descartar el resto evita usarlo como respuesta del siguiente campo. */
        if (strchr(linea, '\n') == NULL && strlen(linea) == sizeof linea - 1) {
            int caracter;
            while ((caracter = getchar()) != '\n' && caracter != EOF) {
            }
            if (ferror(stdin)) {
                return false;
            }
            puts("Entrada demasiado larga. Intente nuevamente.");
            continue;
        }

        char *inicio = linea;
        while (isspace((unsigned char)*inicio)) {
            ++inicio;
        }

        if (isdigit((unsigned char)*inicio)) {
            char *fin;
            errno = 0;
            uintmax_t valor = strtoumax(inicio, &fin, 10);
            int error_conversion = errno;

            while (isspace((unsigned char)*fin)) {
                ++fin;
            }

            if (error_conversion == 0 && *fin == '\0' &&
                valor >= minimo && valor <= maximo) {
                *resultado = (uint64_t)valor;
                return true;
            }
        }

        printf("Entrada invalida. Ingrese un entero entre %" PRIu64
               " y %" PRIu64 ".\n", minimo, maximo);
    }
}

bool configuracion_leer(Configuracion *configuracion) {
    if (configuracion == NULL) {
        return false;
    }

    Configuracion nueva = {0};
    uint64_t opcion;

    if (!leer_entero("Tamano del disco (MB): ", 1, UINT64_MAX / KB_POR_MB,
                     &nueva.tamano_disco_mb)) {
        return false;
    }

    uint64_t capacidad_kb = nueva.tamano_disco_mb * KB_POR_MB;
    if (!leer_entero("Tamano del bloque (KB): ", 1, capacidad_kb,
                     &nueva.tamano_bloque_kb)) {
        return false;
    }

    puts("Mecanismo de asignacion: 1. Contiguo  2. Enlazado  3. Indexado");
    if (!leer_entero("Seleccione una opcion (1-3): ", 1, 3, &opcion)) {
        return false;
    }
    nueva.mecanismo = (MecanismoAsignacion)opcion;

    if (!leer_entero("Tamano minimo de archivo (KB): ", 1, capacidad_kb,
                     &nueva.tamano_min_archivo_kb) ||
        !leer_entero("Tamano maximo de archivo (KB): ",
                     nueva.tamano_min_archivo_kb, capacidad_kb,
                     &nueva.tamano_max_archivo_kb)) {
        return false;
    }

    *configuracion = nueva;
    return true;
}
