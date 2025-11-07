/************************************************************************************************************/
/*  Programa:     lab 04 - Variables privadas en OpenMP                                                     */
/*  Autor:        Carolina Ujueta Ricardo                                                                   */
/*  Fecha:        4 de noviembre de 2025                                                                    */
/*                                                                                                          */
/*  Descripción:                                                                                            */
/*                Este programa demuestra el uso de la cláusula `private` en OpenMP para evitar             */
/*                conflictos al ejecutar bucles en paralelo. Cada hilo posee su propia copia de las         */
/*                variables declaradas como privadas, evitando así condiciones de carrera.                  */
/*                                                                                                          */
/*                Se utiliza la directiva `#pragma omp parallel for` con las cláusulas `private(i)`,        */
/*                `private(a)` y `private(b)` para aislar el estado de las variables dentro de cada hilo.   */
/*                                                                                                          */
/*  Este programa ejemplifica el uso de:                                                                    */
/*   - La directiva `parallel for` para paralelizar un bucle.                                               */
/*   - Las cláusulas `private` para controlar el ámbito de las variables.                                   */
/*   - La eliminación de condiciones de carrera mediante la privatización de datos.                         */
/************************************************************************************************************/

#include <omp.h>     /* Librería principal de OpenMP */
#include <stdio.h>
#include <stdlib.h> 

/************************************************************************************************************/
/*                                                                                                          */
/*  int main()                                                                                              */
/*                                                                                                          */
/*  Propósito:                                                                                              */
/*      Analizar el comportamiento de las variables privadas dentro de una región paralela en OpenMP.       */
/*      Mostrar cómo la declaración de variables privadas previene conflictos entre hilos.                  */
/*                                                                                                          */
/*  Parámetros:                                                                                             */
/*      Ninguno.                                                                                            */
/*                                                                                                          */
/*  Retorno:                                                                                                */
/*      0 si la ejecución finaliza correctamente.                                                           */
/*                                                                                                          */
/************************************************************************************************************/
int main() {

    /* Declaración de variables */
    int i;
    const int N = 1000;
    int a = 50;
    int b = 0;

    /* Región paralela con bucle for */
    #pragma omp parallel for private(i) private(a) private(b)
    for (i = 0; i < N; i++) {
        b = a + i;   /* Cada hilo tiene su propia copia de b */
    }

    /* Resultado */
    printf("a = %d b = %d (Se espera a=50 b=1049)\n", a, b);

    /* Fin de la ejecución */
    return 0;
}

/************************************************************************************************************/
/*    Directiva utilizada:                                                                                  */
/*        #pragma omp parallel for private(i) private(a) private(b)                                         */
/*    Distribuye las iteraciones del bucle entre los hilos, asignando copias privadas                       */
/*     de las variables `i`, `a` y `b` a cada uno.                                                          */
/*                                                                                                          */
/*    Variables:                                                                                            */
/*        i  → privada: índice de iteración independiente por hilo.                                         */
/*        a  → privada: cada hilo tiene su propia copia inicializada a 50.                                  */
/*        b  → privada: cada hilo calcula su propio valor de b sin interferir con otros.                    */
/*                                                                                                          */
/************************************************************************************************************/
