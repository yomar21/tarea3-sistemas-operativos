#include "disco.h"

#include <assert.h>
#include <stdio.h>

static void probar_ocupacion_y_liberacion(void) {
    Disco disco = {0};
    assert(disco_inicializar(&disco, 1, 4));
    assert(disco.capacidad_kb == 1024);
    assert(disco.tamano_bloque_kb == 4);
    assert(disco.cantidad_bloques == 256);
    assert(disco.bloques_libres == 256);
    assert(disco.sobrante_kb == 0);

    for (size_t i = 0; i < disco.cantidad_bloques; ++i) {
        assert(disco_bloque_libre(&disco, i));
        assert(disco_ocupar_bloque(&disco, i));
        assert(!disco_bloque_libre(&disco, i));
        assert(!disco_ocupar_bloque(&disco, i));
        assert(disco.bloques_libres == 255 - i);
        assert(disco_espacio_libre_kb(&disco) == (255 - i) * 4);
    }

    assert(disco_espacio_libre_kb(&disco) == 0);
    assert(!disco_inicializar(&disco, 2, 8));
    assert(disco.cantidad_bloques == 256);
    assert(disco.bloques_libres == 0);

    /* Abrir un hueco en medio y reutilizarlo sin afectar los vecinos. */
    assert(disco_liberar_bloque(&disco, 100));
    assert(!disco_liberar_bloque(&disco, 100));
    assert(disco.bloques_libres == 1);
    assert(disco_espacio_libre_kb(&disco) == 4);
    assert(!disco_bloque_libre(&disco, 99));
    assert(!disco_bloque_libre(&disco, 101));
    assert(disco_ocupar_bloque(&disco, 100));

    for (size_t i = 0; i < disco.cantidad_bloques; ++i) {
        assert(disco_liberar_bloque(&disco, i));
        assert(!disco_liberar_bloque(&disco, i));
        assert(disco.bloques_libres == i + 1);
    }
    assert(disco_espacio_libre_kb(&disco) == 1024);

    assert(!disco_bloque_libre(&disco, 256));
    assert(!disco_ocupar_bloque(&disco, 256));
    assert(!disco_liberar_bloque(&disco, 256));
    assert(!disco_ocupar_bloque(&disco, SIZE_MAX));
    assert(!disco_liberar_bloque(&disco, SIZE_MAX));
    assert(disco.bloques_libres == 256);

    disco_destruir(&disco);
    assert(disco.ocupados == NULL);
    assert(disco.capacidad_kb == 0 && disco.tamano_bloque_kb == 0);
    assert(disco.cantidad_bloques == 0 && disco.bloques_libres == 0);
    assert(disco.sobrante_kb == 0);
    assert(!disco_bloque_libre(&disco, 0));
    assert(!disco_ocupar_bloque(&disco, 0));
    assert(!disco_liberar_bloque(&disco, 0));
    assert(disco_espacio_libre_kb(&disco) == 0);
    disco_destruir(&disco);

    assert(disco_inicializar(&disco, 1, 1024));
    assert(disco.cantidad_bloques == 1);
    assert(disco_ocupar_bloque(&disco, 0));
    assert(disco_espacio_libre_kb(&disco) == 0);
    disco_destruir(&disco);
}

static void probar_sobrante_y_limites(void) {
    Disco disco = {0};
    assert(disco_inicializar(&disco, 1, 3));
    assert(disco.cantidad_bloques == 341);
    assert(disco.sobrante_kb == 1);
    assert(disco_espacio_libre_kb(&disco) == 1023);
    assert(disco_ocupar_bloque(&disco, 340));
    assert(disco_espacio_libre_kb(&disco) == 1020);
    disco_destruir(&disco);

    assert(!disco_inicializar(&disco, 0, 1));
    assert(!disco_inicializar(&disco, 1, 0));
    assert(!disco_inicializar(&disco, 1, 1025));
    assert(!disco_inicializar(&disco, UINT64_MAX / 1024 + 1, 1));
    assert(disco.ocupados == NULL && disco.cantidad_bloques == 0);

    /* Capacidad grande representable con un unico bloque de metadatos. */
    uint64_t maximo_mb = UINT64_MAX / 1024;
    assert(disco_inicializar(&disco, maximo_mb, maximo_mb * 1024));
    assert(disco.cantidad_bloques == 1);
    assert(disco_espacio_libre_kb(&disco) == maximo_mb * 1024);
    disco_destruir(&disco);

    assert(!disco_inicializar(NULL, 1, 1));
    assert(!disco_bloque_libre(NULL, 0));
    assert(!disco_ocupar_bloque(NULL, 0));
    assert(!disco_liberar_bloque(NULL, 0));
    assert(disco_espacio_libre_kb(NULL) == 0);
    disco_destruir(NULL);
}

int main(void) {
    probar_ocupacion_y_liberacion();
    probar_sobrante_y_limites();
    puts("Pruebas de disco superadas.");
    return 0;
}
