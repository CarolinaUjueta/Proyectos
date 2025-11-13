/************************************************************************************************************/
/*  ARCHIVO: mmClasicaPthread.h                                                                             */
/*  AUTOR:  J. Corredor, PhD (documentado por Carolina Ujueta Ricardo, estudiante de Ingeniería de Sistemas)*/
/*  FECHA:  13 Noviembre 2025                                                                               */
/*                                                                                                          */
/*  DESCRIPCIÓN:                                                                                            */
/*  Archivo de encabezado que declara las estructuras, variables globales y prototipos de funciones         */
/*  necesarias para la multiplicación de matrices mediante hilos POSIX (Pthreads).                          */
/*                                                                                                          */
/************************************************************************************************************/

#ifndef MM_CLASICA_POSIX_H
#define MM_CLASICA_POSIX_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

/*------------------------------------------------------------------------------------------
 *  VARIABLES GLOBALES
 *-----------------------------------------------------------------------------------------*/
pthread_mutex_t MM_mutex;
double *matrixA, *matrixB, *matrixC;
struct timeval inicio, fin;

/*------------------------------------------------------------------------------------------
 *  ESTRUCTURA DE PARÁMETROS PARA LOS HILOS
 *-----------------------------------------------------------------------------------------*/
struct parametros {
    int nH;   // Número total de hilos
    int idH;  // Identificador del hilo
    int N;    // Dimensión de la matriz
};

/*------------------------------------------------------------------------------------------
 *  PROTOTIPOS DE FUNCIONES
 *-----------------------------------------------------------------------------------------*/
void InicioMuestra();
void FinMuestra();
void iniMatrix(double *m1, double *m2, int D);
void impMatrix(double *matriz, int D);
void *multiMatrix(void *variables);

#endif
