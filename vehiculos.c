typedef struct {
    int id;
    int direccion; // 0=norte, 1=sur, 2=este, 3=oeste
    float velocidad; // m/s
    float posicion;  // distancia a la intersección
    int en_interseccion; // booleano
} Vehiculo;

/*

Comportamiento del vehiculo
-Avanza hacia la intersección según velocidad y estado del semáforo.
-Si el semáforo está en rojo, se detiene.

*/
