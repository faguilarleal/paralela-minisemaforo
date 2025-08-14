#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>
#include <time.h> 

// -------------------- Estructuras --------------------

typedef struct {
    int id;
    int position;
    int speed;
    int light_id;     // ID del semáforo que controla su carril
    int has_crossed;  // 0 = no, 1 = sí
} Vehicle;

typedef struct {
    int id;
    int state; // 0 = Rojo, 1 = Verde, 2 = Amarillo
} TrafficLight;

typedef struct {
    Vehicle *vehicles;
    int num_vehicles;
    TrafficLight *lights;
    int num_lights;
} Intersection;

char *light_colors[3] = {"Rojo (0)", "Verde (1)", "Amarillo (2)"};
omp_lock_t sem_lock; // es un tipo definido por OpenMP para manejar bloqueos mutuos.

// -------------------- Funciones --------------------

void update_traffic_lights(TrafficLight *lights, int num_lights) {
    #pragma omp parallel for
    for (int i = 0; i < num_lights; i++) {
        if (rand() % 2 == 0) {
            omp_set_lock(&sem_lock);   // bloqueo antes de modificar
            lights[i].state = (lights[i].state + 1) % 3;
            omp_unset_lock(&sem_lock); // desbloqueo
        }
    }
}

void move_vehicles(Vehicle *vehicles, int num_vehicles, TrafficLight *lights) {
    #pragma omp parallel for
    for (int i = 0; i < num_vehicles; i++) {
        if (!vehicles[i].has_crossed) {
            omp_set_lock(&sem_lock);   // bloqueo antes de leer
            int light_state = lights[vehicles[i].light_id].state;
            omp_unset_lock(&sem_lock); // desbloqueo

            if (light_state != 0) { // No está en rojo
                vehicles[i].position += vehicles[i].speed;
                if (vehicles[i].position >= 5) { // 5 = posición de la intersección
                    vehicles[i].has_crossed = 1;
                }
            }
        }
    }
}

void simulate_traffic_dynamic(int num_iterations, Intersection *intersection) {
    omp_set_dynamic(1); // permitir ajuste dinámico de hilos

    for (int i = 0; i < num_iterations; i++) {
        int num_threads = intersection->num_vehicles / 5 + 1;
        omp_set_num_threads(num_threads);

        #pragma omp parallel sections
        {
            #pragma omp section
            update_traffic_lights(intersection->lights, intersection->num_lights);

            #pragma omp section
            move_vehicles(intersection->vehicles, intersection->num_vehicles, intersection->lights);
        }

        // Mostrar estado
        printf("\nIteración %d:\n", i + 1);
        for (int v = 0; v < intersection->num_vehicles; v++) {
            printf("Vehículo %d - Posición: %d Semaforo: %d %s\n",
                   intersection->vehicles[v].id,
                   intersection->vehicles[v].position,
                   intersection->vehicles[v].light_id,
                   intersection->vehicles[v].has_crossed ? "(Cruzó)" : "");
        }
        for (int l = 0; l < intersection->num_lights; l++) {
            printf("Semáforo %d - Estado: %s\n", l, light_colors[intersection->lights[l].state]);
        }

        sleep(1);
    }
}

// -------------------- Main --------------------

int main() {
    srand(1);     
    omp_init_lock(&sem_lock);

    int num_vehicles = 6;
    int num_lights = 2;

    Vehicle *vehicles = malloc(num_vehicles * sizeof(Vehicle));
    TrafficLight *lights = malloc(num_lights * sizeof(TrafficLight));

    // Inicializar semáforos
    for (int i = 0; i < num_lights; i++) {
        lights[i].id = i;
        lights[i].state = rand() % 3;
    }

    // Inicializar vehículos
    for (int i = 0; i < num_vehicles; i++) {
        vehicles[i].id = i;
        vehicles[i].position = 0;
        vehicles[i].speed = (rand() % 3) + 1; // velocidad entre 1 y 3
        vehicles[i].light_id = i % num_lights; // repartir vehículos entre semáforos
        vehicles[i].has_crossed = 0;
    }

    Intersection intersection = {vehicles, num_vehicles, lights, num_lights};

    // Ejecutar simulación
    simulate_traffic_dynamic(17, &intersection);

    // Liberar memoria
    free(vehicles);
    free(lights);

    return 0;
}
