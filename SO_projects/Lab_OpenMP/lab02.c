/************************************************************************************************************/
/*  Programa:     lab 02 - Introducción a OpenMP                                                            */
/*  Autor:        Carolina Ujueta Ricardo                                                                   */
/*  Fecha:        4 de noviembre de 2025                                                                    */
/*                                                                                                          */
/*  Descripción:                                                                                            */
/*                Introducir el uso de la librería OpenMP en C, mostrando cómo                              */
/*                crear regiones paralelas, controlar el número de hilos y                                  */
/*                observar el comportamiento concurrente de los threads.                                    */
/*                                                                                                          */
/*  Este programa ejemplifica el uso básico de la librería OpenMP, incluyendo:                              */
/*   - Lectura de un argumento desde la línea de comandos (número de hilos).                                */
/*   - Configuración dinámica del número de hilos mediante omp_set_num_threads().                           */
/*   - Creación de una región paralela en la que cada hilo imprime su identificador.                        */
/************************************************************************************************************/

#include <omp.h>     /* Librería principal de OpenMP */
#include <stdio.h>
#include <stdlib.h>

/************************************************************************************************************/
/*                                                                                                          */
/*  int main(int argc, char *argv[])                                                                        */
/*                                                                                                          */
/*  Propósito:                                                                                              */
/*      Demostrar el uso de las directivas OpenMP y la configuración de hilos a partir de un argumento.     */
/*                                                                                                          */
/*  Parámetros:                                                                                             */
/*      argc : número de argumentos pasados al programa.                                                    */
/*      argv : arreglo con los argumentos de línea de comandos.                                             */
/*              argv[1] debe contener el número de hilos a utilizar.                                        */
/*                                                                                                          */
/*  Retorno:                                                                                                */
/*      0 si la ejecución finaliza correctamente.                                                           */
/*                                                                                                          */
/************************************************************************************************************/
int main (int argc, char *argv[]) {

    /* Validación de argumentos */
    if (argc != 2) {
        printf("ERROR\n\tUso correcto: $ ./ejecutable numHilos\n");
        exit(0);
    }

    /* Conversión del argumento a entero */
    int numHilos = atoi(argv[1]);
    int maxHilos = omp_get_max_threads();

    /* Información general */
    printf("OpenMP ejecutando en cores = %d hilos disponibles\n", maxHilos);

    /* Configuración del número de hilos */
    omp_set_num_threads(numHilos);

    /* Región paralela */
    #pragma omp parallel
    {
        printf("Hello world desde el thread %d\n", omp_get_thread_num());
    }

    /* Fin de la ejecución */
    return 0;
}
