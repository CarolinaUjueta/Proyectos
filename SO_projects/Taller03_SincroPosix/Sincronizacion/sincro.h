/************************************************************************************************
*                               Pontificia Universidad Javeriana                                *
*                                                                                               *
* Autor:     Thomas Leal PUerta, Carolina Ujueta Ricardo                                        *
* Fecha:     13/11/2025                                                                         *
* Materia:   Sistemas Operativos                                                                *
* Profesor:  John Jairo Corredor Franco                                                         *
* Objetivo:  Declarar las estructuras, constantes y prototipos de las funciones relacionadas    *
*            con el sistema productor–spooler usando hilos y variables de condicion.            *
*************************************************************************************************
* HOW TO USE                                                                                    *
* ----------------------------------------------------------------------------------------------*
* 1. Incluir este archivo en los modulos que usen el sistema de impresion en bufer:             *
*        #include "producer_spooler.h"                                                          *
* 2. Compilar junto con producer_spooler.c y main.c usando pthreads:                            *
*        gcc -pthread main.c sincro.c -o sincro                                                 *
*************************************************************************************************/

#ifndef __SINCRO_H__
#define __SINCRO_H__

/**************************************** Headers **********************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**************************************** Constantes *********************************************/

#define MAX_BUFFERS 10   /* Puede ajustarse si el codigo original define otro valor */

/*************************************** Declaracion Funciones ***********************************/

/*************************************************************************************************
 * Firma:     void *producer (void *arg)                                                         *
 * Proposito: Funcion hilo productor. Escribe lineas de texto en los buferes compartidos.        *
 * Parametros:                                                                                   *
 *   - arg: puntero a entero que identifica el numero del hilo productor.                        *
 *************************************************************************************************/
void *producer (void *arg);

/************************************************************************************************
 * Firma:     void *spooler (void *arg)                                                         *
 * Proposito: Funcion hilo spooler. Extrae lineas del bufer y las imprime en pantalla.          *
 * Parametros:                                                                                  *
 *   - arg: no se utiliza (puede ser NULL).                                                     *
 ************************************************************************************************/
void *spooler (void *arg);

#endif /* __SINCRO_H__ */
