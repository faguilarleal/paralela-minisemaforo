# Informe: Simulación de Semáforos en Paralelo con OpenMP


## Instrucciones para correr el programa

```commandline
gcc -fopenmp -o simulacion main.c
```

y posteriormente corremos:
```commandline
./simulacion
```

## 1. Estructuras de Datos

1. **Vehicle (Vehículo):**  
   Representa cada automóvil en la intersección. Sus campos son:
   - `id`: identificador único del vehículo.  
   - `position`: posición actual en el carril (un número entero).  
   - `speed`: velocidad del vehículo, entre 1 y 3.  
   - `light_id`: referencia al semáforo que controla su carril.  
   - `has_crossed`: indicador de si ya cruzó la intersección (0 = no, 1 = sí).  

2. **TrafficLight (Semáforo):**  
   Representa un semáforo en la intersección. Sus campos son:  
   - `id`: identificador único del semáforo.  
   - `state`: estado actual del semáforo (0 = Rojo, 1 = Verde, 2 = Amarillo).  

3. **Intersection (Intersección):**  
   Representa la intersección completa con vehículos y semáforos:
   - `vehicles`: arreglo de vehículos que participan en la simulación.  
   - `num_vehicles`: número total de vehículos.  
   - `lights`: arreglo de semáforos.  
   - `num_lights`: número total de semáforos.  

Estas estructuras permiten modelar de manera clara la relación entre vehículos y semáforos y mantener su estado actualizado en la simulación.

---

## 2. Uso de Paralelismo

Se utiliza OpenMP para acelerar la simulación mediante **paralelismo de hilos**:

1. **Paralelismo interno (dentro de cada función)**  
   - `update_traffic_lights()` usa `#pragma omp parallel for` para actualizar múltiples semáforos en paralelo.  
   - `move_vehicles()` usa `#pragma omp parallel for` para mover varios vehículos simultáneamente.  
   
   Esto permite que cada semáforo y cada vehículo se actualicen en paralelo, reduciendo el tiempo total de ejecución.

2. **Paralelismo entre funciones (secciones)**  
   ```c
   #pragma omp parallel sections
   {
       #pragma omp section
       update_traffic_lights(...);

       #pragma omp section
       move_vehicles(...);
   }

Aquí se ejecutan simultáneamente dos tareas distintas:

* La actualización de los semáforos.
* El movimiento de los vehículos.

Cada sección puede correr en un hilo diferente, maximizando el aprovechamiento de los núcleos de la CPU.

## 3. Uso del Mutex (Lock)

Se define un mutex (omp_lock_t sem_lock) para proteger el acceso a los semáforos.

* _Problema sin mutex_: Si un hilo lee el estado de un semáforo mientras otro hilo lo está modificando podría obtener un valor inconsistente.

Solución:

`omp_set_lock(&sem_lock)` antes de leer o escribir el estado del semáforo.

`omp_unset_lock(&sem_lock)` después de completar la operación.

Esto asegura que solo un hilo a la vez acceda al semáforo, evitando condiciones de carrera y garantizando la consistencia de la simulación.

4. Rol del Paralelismo Dinámico

`omp_set_dynamic(1)` permite que OpenMP ajuste automáticamente el número de hilos según la carga de trabajo.

El número de hilos se calcula en cada iteración con:

```c
int num_threads = intersection->num_vehicles / 5 + 1;
omp_set_num_threads(num_threads);
```

Esto es útil porque:

* Cuando hay pocos vehículos, no se desperdician hilos.

* Cuando hay muchos vehículos, se pueden usar más hilos para acelerar el procesamiento de move_vehicles().

El paralelismo dinámico mejora la eficiencia y escalabilidad, adaptando el uso de recursos a la cantidad de vehículos presentes.

## 5. Flujo de la Simulación

1. Inicialización de semáforos y vehículos con valores aleatorios fijos (srand(1) para reproducibilidad).

2. Iteraciones de simulación: Se ejecutan en paralelo las secciones:

* Actualización de semáforos.

* Movimiento de vehículos según la luz correspondiente.

* Después de que ambas secciones terminan (sincronización implícita), se imprime el estado actual.

3. La salida muestra:

* Posición de cada vehículo.

* Semáforo que controla su carril.

* Estado de los semáforos.

* Si un vehículo ha cruzado la intersección.

## 6. Conclusión

La simulación combina varias técnicas de paralelismo:

Paralelismo interno para actualizar semáforos y vehículos simultáneamente.

Paralelismo de secciones para que la actualización de semáforos y el movimiento de vehículos ocurran al mismo tiempo.

Locks (mutex) para garantizar consistencia en el acceso a los semáforos.

Paralelismo dinámico para ajustar la cantidad de hilos a la carga de trabajo, mejorando eficiencia.

Esto permite una simulación realista y eficiente de tráfico en una intersección controlada por semáforos, manteniendo la coherencia de los datos y aprovechando múltiples núcleos de CPU.