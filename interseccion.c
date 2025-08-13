#include "interseccion.h"
#include <stdio.h>

void inicializarInterseccion(Interseccion* inter) {
    for (int i = 0; i < 4; i++) {
        inter->semaforos[i].estado = ROJO;
    }
    printf("Interseccion inicializada con semaforos en ROJO\n");
}
