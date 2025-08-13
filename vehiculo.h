#ifndef VEHICULO_H
#define VEHICULO_H

typedef struct {
    int id;
    int direccion; // 0=norte, 1=sur, 2=este, 3=oeste
    float velocidad;
    float posicion;
    int en_interseccion;
} Vehiculo;

#endif

/*

Comportamiento del vehiculo
-Avanza hacia la intersección según velocidad y estado del semáforo.
-Si el semáforo está en rojo, se detiene.

*/
