
typedef enum { ROJO, AMARILLO, VERDE } EstadoSemaforo;

typedef struct {
    int id;
    EstadoSemaforo estado;
    int tiempo_verde;
    int tiempo_amarillo;
    int tiempo_rojo;
    int tiempo_actual;
} Semaforo;

/*
Comportamiento del semaforo 

-Cambia de estado cíclicamente según temporizadores.
-Cada dirección puede tener su propio semáforo o estar sincronizados.

*/