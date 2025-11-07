/************************************************************************************************************/
/*  Programa:     Lab 03 - Variables privadas y compartidas en OpenMP                                       */
/*  Autor:        Carolina Ujueta Ricardo                                                                   */
/*  Fecha:        4 de noviembre de 2025                                                                    */
/*                                                                                                          */
/*  Descripción:                                                                                            */
/*                Este programa demuestra el comportamiento de las variables privadas y compartidas         */
/*                dentro de una región paralela utilizando OpenMP.                                          */
/*                Se utiliza la directiva `#pragma omp parallel for` para distribuir las iteraciones         */
/*                de un bucle entre múltiples hilos.                                                        */
/*                                                                                                          */
/*                El objetivo es analizar cómo las cláusulas `private` afectan el alcance de las variables  */
/*                y cómo una variable compartida puede provocar una condición de carrera.                   */
/*                                                                                                          */
/*  Este programa ejemplifica el uso de:                                                                    */
/*   - Directiva `parallel for` con cláusulas `private(i)` y `private(a)`.                                  */
/*   - Variables compartidas entre hilos (b).                                                               */
/*   - Ejemplo de condición de carrera al modificar una variable global sin sincronización.                 */
/************************************************************************************************************/

#include <omp.h>     /* Librería principal de OpenMP */
#include <stdio.h>
#include <stdlib.h>

/************************************************************************************************************/
/*                                                                                                          */
/*  int main()                                                                                              */
/*                                                                                                          */
/*  Propósito:                                                                                              */
/*      Analizar el efecto del paralelismo en el acceso a variables locales y globales dentro de un bucle.  */
/*      Mostrar cómo el uso incorrecto de variables compartidas puede producir condiciones de carrera.      */
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
    #pragma omp parallel for private(i) private(a)
    for (i = 0; i < N; i++) {
        b = a + i;   /* b es compartida: posible condición de carrera */
    }

    /* Resultado */
    printf("a = %d b = %d (Se espera a=50 b=1049)\n", a, b);

    /* Fin de la ejecución */
    return 0;
}

/*************************************************************************************************************/
/*   Directiva utilizada:                                                                                    */
/*        #pragma omp parallel for private(i) private(a)                                                     */
/*    Divide las iteraciones del bucle entre los hilos del equipo,                                           */
/*    garantizando copias independientes de las variables privadas.                                          */
/*                                                                                                           */
/*   Variables:                                                                                              */
/*        i  → privada (cada hilo usa su propio índice).                                                     */
/*        a  → privada (cada hilo tiene una copia local).                                                    */
/*        b  → compartida (todos los hilos escriben sobre la misma dirección de memoria).                    */
/*************************************************************************************************************/
