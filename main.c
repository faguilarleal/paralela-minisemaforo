#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // sleep()
#include "interseccion.h"
#include "vehiculo.h"

// Función para cambiar el estado de un semáforo en ciclo
void actualizarSemaforo(Semaforo *s) {
    s->tiempo_actual++;
    if (s->estado == VERDE && s->tiempo_actual >= s->tiempo_verde) {
        s->estado = AMARILLO;
        s->tiempo_actual = 0;
    } else if (s->estado == AMARILLO && s->tiempo_actual >= s->tiempo_amarillo) {
        s->estado = ROJO;
        s->tiempo_actual = 0;
    } else if (s->estado == ROJO && s->tiempo_actual >= s->tiempo_rojo) {
        s->estado = VERDE;
        s->tiempo_actual = 0;
    }
}

// Imprimir estado de todos los semáforos
void imprimirSemaforos(Interseccion *inter) {
    const char *nombres[] = {"NORTE", "SUR", "ESTE", "OESTE"};
    const char *colores[] = {"ROJO", "AMARILLO", "VERDE"};
    for (int i = 0; i < 4; i++) {
        printf("Semaforo %s: %s | ", nombres[i], colores[inter->semaforos[i].estado]);
    }
    printf("\n");
}

// Mover los vehículos si tienen luz verde
void moverVehiculos(Interseccion *inter) {
    for (int i = 0; i < inter->num_vehiculos; i++) {
        Vehiculo *v = &inter->vehiculos[i];
        if (inter->semaforos[v->direccion].estado == VERDE) {
            moverVehiculo(v, 1.0f);
        } else {
            printf("Vehiculo %d detenido en semaforo %d\n", v->id, v->direccion);
        }
    }
}

int main() {
    // Crear 8 vehículos (2 por dirección)
    Vehiculo vehiculos[8];
    int id = 1;
    for (int dir = 0; dir < 4; dir++) {
        for (int j = 0; j < 2; j++) {
            vehiculos[id - 1].id = id;
            vehiculos[id - 1].direccion = dir;
            vehiculos[id - 1].velocidad = 1.0f;
            vehiculos[id - 1].posicion = 10.0f; // a 10 unidades de la intersección
            vehiculos[id - 1].en_interseccion = 0;
            id++;
        }
    }

    // Crear intersección
    Interseccion inter;
    inter.vehiculos = vehiculos;
    inter.num_vehiculos = 8;

    inicializarInterseccion(&inter);

    // Configurar semáforos con tiempos (en segundos)
    for (int i = 0; i < 4; i++) {
        inter.semaforos[i].id = i;
        inter.semaforos[i].tiempo_verde = 3;
        inter.semaforos[i].tiempo_amarillo = 1;
        inter.semaforos[i].tiempo_rojo = 4; // para ciclo simple
        inter.semaforos[i].tiempo_actual = 0;
        inter.semaforos[i].estado = (i % 2 == 0) ? VERDE : ROJO; // Norte y Este empiezan en verde
    }

    // Simulación de 15 segundos
    for (int t = 0; t < 15; t++) {
        printf("\n--- SEGUNDO %d ---\n", t + 1);
        imprimirSemaforos(&inter);
        moverVehiculos(&inter);

        // Actualizar semáforos
        for (int i = 0; i < 4; i++) {
            actualizarSemaforo(&inter.semaforos[i]);
        }

        sleep(1);
    }

    return 0;
}
