/* =========================================================================
   Programa: Ejecución paralela con OpenMP
   Autor: Carolina Ujueta Ricardo
   Descripción:
       Demostración básica del uso de la biblioteca OpenMP en C.
       Este programa ejecuta un bloque de código en paralelo, 
       donde cada hilo imprime su identificador (ID de thread).

       El objetivo es la creación de hilos paralelos
       y obtener el número de hilos activos.
   ======================================================================== */

#include <omp.h>     /* Librería principal de OpenMP */
#include <stdio.h>
#include <stdlib.h> 

/************************************************************************************************************/
/*                                                                                                          */
/*  int main(int argc, char *argv[])                                                                        */
/*                                                                                                          */
/*  Propósito: Crear una región paralela usando OpenMP y mostrar información                                */
/*             de los hilos que participan en la ejecución.                                                 */
/*                                                                                                          */
/*  Parámetros:                                                                                             */
/*      argc : número de argumentos pasados al programa.                                                    */
/*      argv : arreglo con los argumentos de línea de comandos.                                             */
/*                                                                                                          */
/*  Retorno:                                                                                                */
/*      0 si la ejecución finaliza correctamente.                                                           */
/*                                                                                                          */
/************************************************************************************************************/
int main(int argc, char *argv[]) {

    /* ---- Información general del entorno OpenMP ---- */
    printf("OpenMP ejecutando con %d hilos\n", omp_get_max_threads());
    /*  omp_get_max_threads(): devuelve el número máximo de hilos
        disponibles para la ejecución paralela. */

    /* ---- Región paralela ---- */
    #pragma omp parallel
    {
        /* Cada hilo imprime su identificador único. */
        printf("Hello Word desde el thread %d\n", omp_get_thread_num());
        /*  omp_get_thread_num(): devuelve el ID del hilo actual
            dentro del equipo de hilos (desde 0 hasta N-1). */
    }

    /* ---- Fin de la ejecución ---- */
    return 0;
}
/************************************************************************************************************/
/*                                                                                                          */
/*    Condición de carrera:                                                                                 */
/*          Este programa presenta una condición de carrera leve en las llamadas a printf() dentro          */
/*          de la región paralela. Cada hilo escribe de forma concurrente en la salida estándar,            */
/*          lo que puede causar entrelazado de los mensajes (salidas desorganizadas).                       */
/*          Para evitar el entrelazado, se puede usar:                                                      */
/*                                                                                                          */
/*              #pragma omp critical                                                                        */
/*              { printf("Hello Word desde el thread %d\n", omp_get_thread_num()); }                        */
/*                                                                                                          */
/*         Esto garantiza que solo un hilo imprime a la vez, eliminando el riesgo de condición de carrera.  */
/*                                                                                                          */
/************************************************************************************************************/
