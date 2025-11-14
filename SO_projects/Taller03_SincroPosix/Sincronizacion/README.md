# Actividad 2 — POSIX Threads (Productores–Spooler)  
Pontificia Universidad Javeriana  
Profesor: John Jairo Corredor, PhD  
Autor: Carolina Ujueta Ricardo  
Fecha: 13 Noviembre de 2025  

## Introducción
Esta actividad demuestra el uso de **pthreads** para ejecutar múltiples flujos dentro de un mismo proceso.  
Los hilos tienen pilas propias pero **comparten datos globales** (búfer circular e índices).  
La sincronización se garantiza mediante **exclusión mutua** (mutex) y **variables de condición** para evitar que:
- un hilo lea/escriba datos mientras otro los modifica, y
- se desperdicie CPU esperando estados compartidos.

## Requisitos
- Sistema operativo Linux.
- Compilador `gcc` con soporte de `pthread`.
- Archivo fuente: `posixSincro.c`.

## Actividad 2 – Sincronización de hilos con mutex y condición
**Objetivo:**  
Coordinar **productores** y **spooler (consumidor)** sobre un **búfer circular** compartido, asegurando integridad del estado y progreso sin espera activa.

**Conceptos aplicados:**  
- Creación y gestión de hilos: `pthread_create`, `pthread_join`, `pthread_cancel`.  
- Exclusión mutua: `pthread_mutex_t` y `pthread_mutex_lock/unlock`.  
- Sincronización por condición: `pthread_cond_t`, `pthread_cond_wait`, `pthread_cond_signal`.  
- Búfer circular, contadores compartidos y sección crítica.  
- Evitar condiciones de carrera y esperas improductivas.

**Compilación y ejecución:**  
gcc -o posixSincro.c -o sincro
