/************************************************************************************************
*                               Pontificia Universidad Javeriana                                *
*                                                                                               *
* Autor:     Thomas Leal PUerta, Carolina Ujueta Ricardo                                        *
* Fecha:     13/11/2025                                                                         *
* Materia:   Sistemas Operativos                                                                *
* Objetivo:  Definir la estructura de datos compartida y el tamanno del bufer para productor y  *
*            consumidor utilizando memoria compartida.                                          *
************************************************************************************************/

#ifndef __SEMAFORO_H__
#define __SEMAFORO_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

/* ------------------------------ Constantes del bufer ------------------------------ */
#define BUFFER 5

/* --------------------------- Estructura de datos comun --------------------------- */
/* ---- Estructura que modela el bufer circular compartido entre procesos ---- */
typedef struct {
    int bus[BUFFER];   /* ---- Arreglo que almacena los elementos del bufer ---- */
    int entrada;       /* ---- indice de escritura (productor) ---- */
    int salida;        /* ---- indice de lectura (consumidor) ---- */
} compartir_datos;

#endif /* __SEMAFORO_H__ */
