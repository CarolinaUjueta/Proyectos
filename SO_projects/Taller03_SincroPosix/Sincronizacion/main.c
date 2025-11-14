/************************************************************************************************
*                               Pontificia Universidad Javeriana                                *
*                                                                                               *
* Autor:     Thomas Leal PUerta, Carolina Ujueta Ricardo                                        *
* Fecha:     13/11/2025                                                                         *
* Materia:   Sistemas Operativos                                                                *
* Profesor:  John Jairo Corredor Franco                                                         *
* Objetivo:  Crear y gestionar los hilos productores y el hilo spooler que comparten buferes    *
*            de impresion utilizando pthreads.                                                  *
*************************************************************************************************
* HOW TO USE                                                                                    *
* --------------------------------------------------------------------------------------------- *
* 1. Compilar el proyecto:                                                                      *
*                                                                                               *
* 2. Ejecutar:                                                                                  *
*                                                                                               *
* 3. Observar en pantalla como los productores generan lineas y el spooler las imprime.         *
*************************************************************************************************/

/***************************************** Headers **********************************************/

#include "sincro.h"

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
