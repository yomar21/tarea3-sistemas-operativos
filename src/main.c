#include "configuracion.h"

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

    return EXIT_SUCCESS;
}
