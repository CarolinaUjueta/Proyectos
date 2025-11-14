/************************************************************************************************
*                               Pontificia Universidad Javeriana                                *
*                                                                                               *
* Autor:     Thomas Leal PUerta, Carolina Ujueta Ricardo                                        *
* Fecha:     13/11/2025                                                                         *
* Materia:   Sistemas Operativos                                                                *
* Profesor:  John Jairo Corredor Franco                                                         *
* Objetivo:  Implementar las funciones productor y spooler que comparten un conjunto de buferes *
*            protegidos con mutex y variables de condicion usando pthreads.                     *
*************************************************************************************************
* HOW TO USE                                                                                    *
* --------------------------------------------------------------------------------------------- *
* 1. Compilar junto con main.c y este archivo:                                                  *
*        gcc -Wall -pthread main.c producer_spooler.c -o spooler_app                            *
* 2. Ejecutar el programa generado:                                                             *
*        ./spooler_app                                                                          *
* 3. Diez hilos productores generaran mensajes que seran impresos por el hilo spooler.          *
*************************************************************************************************/

/***************************************** Headers **********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "sincro.h"

/*************************************** Variables Globales ***********************************/

/* ---- Arreglo de buferes de salida ---- */
char buf [MAX_BUFFERS] [100];
int buffer_index;
int buffer_print_index;

/* ---- Sincronizacion entre productores y spooler ---- */
pthread_mutex_t buf_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buf_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t spool_cond = PTHREAD_COND_INITIALIZER;

/* ---- Contadores de estado ---- */
int buffers_available = MAX_BUFFERS;
int lines_to_print = 0;

/*************************************** Declaracion Funciones ***********************************/

/*************************************************************************************************
 * Firma:     void *producer (void *arg)                                                         *
 * Proposito: Hilo productor. Escribe mensajes en los buferes cuando haya espacio disponible.    *
 * Parametros:                                                                                   *
 *   - arg: puntero a entero que indica el identificador logico del hilo productor.              *
 *************************************************************************************************/
void *producer (void *arg){
    int i, r;
    int my_id = *((int *) arg);
    int count = 0;

    for (i = 0; i < 10; i++) {

        /* ---- Seccion critica: solicitar acceso al bufer compartido ---- */
        if ((r = pthread_mutex_lock (&buf_mutex)) != 0) {
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
        }
            /* ---- Esperar mientras no haya buferes disponibles ---- */
            while (!buffers_available) 
                pthread_cond_wait (&buf_cond, &buf_mutex);

            /* ---- Reservar posicion en el bufer circular ---- */
            int j = buffer_index;
            buffer_index++;
            if (buffer_index == MAX_BUFFERS)
                buffer_index = 0;
            buffers_available--;

            /* ---- Escribir mensaje en el bufer y actualizar contador de lineas ---- */
            sprintf (buf [j], "Thread %d: %d\n", my_id, ++count);
            lines_to_print++;

            /* ---- Despertar al spooler para que procese una nueva linea ---- */
            pthread_cond_signal (&spool_cond);

        /* ---- Salir de la seccion critica ---- */
        if ((r = pthread_mutex_unlock (&buf_mutex)) != 0) {
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
        }
    
        /* ---- Pausa entre producciones para simular trabajo ---- */
        sleep (1);
    }
}

/************************************************************************************************
 * Firma:     void *spooler (void *arg)                                                         *
 * Proposito: Hilo spooler (consumidor). Toma lineas del bufer y las imprime en stdout.         *
 * Parametros:                                                                                  *
 *   - arg: no se usa (puede ser NULL).                                                         *
 ************************************************************************************************/
void *spooler (void *arg){
    int r;

    while (1) {  
        /* ---- Seccion critica: acceso al bufer compartido ---- */
        if ((r = pthread_mutex_lock (&buf_mutex)) != 0) {
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
        }
            /* ---- Esperar a que haya al menos una linea para imprimir ---- */
            while (!lines_to_print) 
                pthread_cond_wait (&spool_cond, &buf_mutex);

            /* ---- Imprimir la linea correspondiente del bufer circular ---- */
            printf ("%s", buf [buffer_print_index]);
            lines_to_print--;

            /* ---- Avanzar indice de impresion en el bufer circular ---- */
            buffer_print_index++;
            if (buffer_print_index == MAX_BUFFERS)
               buffer_print_index = 0;

            /* ---- Liberar un bufer y notificar a productores ---- */
            buffers_available++;
            pthread_cond_signal (&buf_cond);

        /* ---- Salir de la seccion critica ---- */
        if ((r = pthread_mutex_unlock (&buf_mutex)) != 0) {
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
        }

    }
}
