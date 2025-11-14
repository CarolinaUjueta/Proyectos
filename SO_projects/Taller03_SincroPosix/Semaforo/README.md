# Actividad 1 — POSIX Semáforos (Productor/Consumidor)  
Pontificia Universidad Javeriana  
Profesor: John Jairo Corredor, PhD  
Autor: Thomas Leal Puerta  
Fecha: 13 Noviembre de 2025  

## Introducción
Esta actividad valida el patrón **Productor–Consumidor** empleando **semaforía POSIX con nombre** y **memoria compartida**.  
Se realizan dos pruebas: (A) el productor produce **solo 5 elementos** iniciales (llenado limitado del búfer) y (B) al iniciar el consumidor, los elementos se **consumen mientras** el productor **continúa enviando** hasta completar su secuencia.

## Requisitos
- Sistema operativo Linux con soporte POSIX.
- Compilador `gcc`.
- Cabeceras y librerías: `semaphore.h`, `fcntl.h`, `sys/mman.h`, `unistd.h`.

## Componentes
- `producer`: crea/abre semáforos y memoria compartida; inserta elementos en el búfer circular.
- `consumer`: abre los objetos creados por el productor; extrae elementos del búfer circular.

## Compilación y ejecución
# Compilar
gcc producer.c -o producer -lrt -lpthread
gcc consumer.c -o consumer -lrt -lpthread

# Prueba A: solo productor (llenado limitado a 5)
./producer

# Prueba B: con productor ya ejecutado, iniciar consumidor en otra terminal
./consumer
Prueba A — Llenado inicial limitado (productor produce solo 5)
Objetivo
Evidenciar que el productor respeta la cota inicial del búfer y se detiene tras producir cinco elementos cuando no hay consumidor activo.

Ejecución
Se lanza únicamente ./producer. En consola se observan exactamente cinco líneas:

makefile
Copiar código
Productor: Produce 1
Productor: Produce 2
Productor: Produce 3
Productor: Produce 4
Productor: Produce 5
Criterio de aceptación
La producción se detiene visualmente en 5 registros, confirmando el control de capacidad del búfer mediante semáforos.

Prueba B — Consumo en curso con producción continua
Objetivo
Verificar que, al iniciarse el consumidor, los elementos se consumen y el productor reanuda la inserción hasta completar su secuencia (10).

Ejecución
Con el productor ya ejecutado (Prueba A), se inicia ./consumer en otra terminal.
Salidas típicas:

Productor

makefile
Copiar código
Productor: Produce 1
...
Productor: Produce 5
Productor: Produce 6
Productor: Produce 7
Productor: Produce 8
Productor: Produce 9
Productor: Produce 10
Consumidor

makefile
Copiar código
Consumidor: Consume 1
Consumidor: Consume 2
...
Consumidor: Consume 10
Criterio de aceptación
Se observa alternancia estable: el consumidor libera espacio y el productor completa 6..10 sin pérdidas ni desbordes.

Conclusión
Las dos pruebas confirman el comportamiento esperado del esquema productor–consumidor con semáforos POSIX:
(1) Control de capacidad efectivo (detención tras 5 elementos sin consumidor) y
(2) Flujo coordinado cuando el consumidor entra en operación, consumiéndose los elementos a medida que se producen hasta completar la secuencia.
