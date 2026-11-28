#include "configuracion.h"
#include "disco.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    printf("Tarea 3 - Sistemas Operativos\n");
    printf("Simulador de asignacion de archivos\n");
    Configuracion configuracion;
    if (!configuracion_leer(&configuracion)) {
        fprintf(stderr, "\nNo se pudo completar la configuracion: "
                        "la entrada termino o no pudo leerse.\n");
        return EXIT_FAILURE;
    }

    const char *nombres_mecanismos[] = {"Contiguo", "Enlazado", "Indexado"};
    printf("\nConfiguracion registrada:\n");
    printf("Disco: %" PRIu64 " MB\n", configuracion.tamano_disco_mb);
    printf("Bloque: %" PRIu64 " KB\n", configuracion.tamano_bloque_kb);
    printf("Mecanismo: %s\n", nombres_mecanismos[configuracion.mecanismo - 1]);
    printf("Archivos: %" PRIu64 " a %" PRIu64 " KB\n",
           configuracion.tamano_min_archivo_kb,
           configuracion.tamano_max_archivo_kb);

    Disco disco = {0};
    if (!disco_inicializar(&disco, configuracion.tamano_disco_mb,
                           configuracion.tamano_bloque_kb)) {
        fprintf(stderr, "No se pudo inicializar el disco: tamanos no "
                        "representables o memoria insuficiente.\n");
        return EXIT_FAILURE;
    }

    printf("\nDisco inicializado:\n");
    printf("Bloques totales: %zu\n", disco.cantidad_bloques);
    printf("Bloques libres: %zu\n", disco.bloques_libres);
    printf("Bloques ocupados: %zu\n",
           disco.cantidad_bloques - disco.bloques_libres);
    printf("Espacio disponible: %" PRIu64 " KB\n", disco_espacio_libre_kb(&disco));
    printf("Sobrante fuera de los bloques: %" PRIu64 " KB\n", disco.sobrante_kb);

    disco_destruir(&disco);
    return EXIT_SUCCESS;
}
