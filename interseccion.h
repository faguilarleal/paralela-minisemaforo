

#ifndef INTERSECCION_H
#define INTERSECCION_H

#include "semaforo.h"
#include "vehiculo.h"

typedef struct {
    Semaforo semaforos[4]; // norte, sur, este, oeste
    Vehiculo* vehiculos;
    int num_vehiculos;
} Interseccion;

#endif


/*
Comportamiento 

-Coordina semáforos.
-Determina qué vehículos pueden pasar.
-Detecta colisiones (si las simulas).

*/