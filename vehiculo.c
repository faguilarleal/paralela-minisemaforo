#include "vehiculo.h"
#include <stdio.h>

void moverVehiculo(Vehiculo* v, float delta) {
    if (!v->en_interseccion) {
        v->posicion -= v->velocidad * delta;
        printf("Vehiculo %d movido a posicion %.2f\n", v->id, v->posicion);
    }
}
