/************************************************************************************************
*                               Pontificia Universidad Javeriana                                *
*                                                                                               *
* Autor:     Thomas Leal PUerta, Carolina Ujueta Ricardo                                        *
* Fecha:     13/11/2025                                                                         *
* Materia:   Sistemas Operativos                                                                *
* Profesor:  John Jairo Corredor Franco                                                         *
* Objetivo:  Crear y gestionar los hilos productores y el hilo spooler que comparten buferes    *
*            de impresion utilizando pthreads.                                                  *
*                                                                                               *
* Descripcion:                                                                                  *
* El programa implementa el modelo Productor–Consumidor usando 10 hilos productores y un hilo   *
* spooler. Todos comparten un búfer circular de 100 posiciones donde los productores escriben   *
* mensajes y el spooler los imprime en orden.                                                   *
*                                                                                               *
* La sincronización evita que:                                                                  *
*   - Un productor escriba cuando no hay espacio disponible.                                    *
*   - El spooler lea cuando no hay líneas por imprimir.                                         *
*                                                                                               *
* Se emplean:                                                                                   *
*   • Mutex: protege índices y contadores del búfer.                                            *
*   • pthread_cond_t buf_cond: despierta productores cuando hay espacio.                        *
*   • pthread_cond_t spool_cond: despierta al spooler cuando hay nuevas líneas.                 *
*************************************************************************************************
* HOW TO USE                                                                                    *
* --------------------------------------------------------------------------------------------- *
* 1. Compilar el proyecto:                                                                      *
*      gcc main.c -o sincro                                                                     *
* 2. Ejecutar:                                                                                  *
*      ./sincro                                                                                 *
*************************************************************************************************/

/***************************************** Headers **********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

/******************************************* Constantes ****************************************/

#define MAX_BUFFERS 100 

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
/* **********************************************************************************************
 * Firma:     int main (int argc, char **argv)                                                  *
 * Proposito: Punto de entrada del programa. Inicializa los indices, crea los hilos productores *
 *            y el hilo spooler, espera a que los productores terminen y cancela al spooler.    *
 * Parametros:                                                                                  *
 *   - argc: numero de argumentos de linea de comandos (no se usan).                            *
 *   - argv: arreglo de cadenas con los argumentos (no se usan).                                *
 * Retorno:                                                                                     *
 *   - int: codigo de salida del programa (0 si todo fue correcto).                             *
 ************************************************************************************************/
int main (int argc, char **argv){
    
    /* ---- Identificadores de hilos: 10 productores y 1 spooler ---- */
    pthread_t tid_producer [10], tid_spooler;
    
    /* ---- Variables auxiliares: i para bucles, r para codigos de retorno ---- */
    int i, r;

    /* ---- Inicializar indices del buffer circular ---- */
    buffer_index = buffer_print_index = 0;
    
    /* ---- Crear hilo spooler (consumidor/imprimiendo lineas) ---- */
    if ((r = pthread_create (&tid_spooler, NULL, spooler, NULL)) != 0) {
        
        /* ---- Reportar error si falla la creacion del spooler ---- */
        fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
        
    }
    /* ---- Identificadores logicos para productores (0..9) ---- */
    int thread_no [10];
    
    /* ---- Crear los 10 hilos productores, pasando su id por argumento ---- */
    for (i = 0; i < 10; i++) {
        
        thread_no [i] = i;
        
        if ((r = pthread_create (&tid_producer [i], NULL, producer, (void *) &thread_no [i])) != 0) {
            
            /* ---- Reportar error si falla la creacion de algun productor ---- */
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
            
        }
    
    }
    /* ---- Esperar a que terminen todos los productores ---- */
    for (i = 0; i < 10; i++)
    {       
        
        if ((r = pthread_join (tid_producer [i], NULL)) == -1) {
            
            /* ---- Reportar error si falla el join de algun productor ---- */
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
        }
        
    }
    
    /* ---- Esperar a que el spooler vacie todas las lineas pendientes ---- */
    while (lines_to_print) sleep (1);
    
    /* ---- Cancelar el hilo spooler (ya no hay trabajo) ---- */
    if ((r = pthread_cancel (tid_spooler)) != 0) {
        
        /* ---- Reportar error si falla la cancelacion del spooler ---- */
        fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);
        
    }

    /* ---- Finalizacion correcta del proceso principal ---- */
    exit (0);
}
