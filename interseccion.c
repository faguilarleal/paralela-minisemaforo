

typedef struct {
    Semaforo semaforos[4]; // norte, sur, este, oeste
    Vehiculo* vehiculos;
    int num_vehiculos;
} Interseccion;


/*
Comportamiento 

-Coordina semáforos.
-Determina qué vehículos pueden pasar.
-Detecta colisiones (si las simulas).

*/