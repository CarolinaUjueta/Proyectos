/*****************************************************************************************************
 *                                   PONTIFICIA UNIVERSIDAD JAVERIANA                                *
 *                     Departamento de Ingeniería de Sistemas – Sistemas Operativos                  *
 *                                                                                                   *
 * ------------------------------------------------------------------------------------------------- *
 * Autor       : Thomas Leal, Carolina Ujueta                                                        *
 * Fecha       : 27/10/2025                                                                          *
 * Objetivo    : 4 procesos con fork(); cada hijo lee su archivo y calcula la suma total de enteros. *
 *               Padre recibe por pipe el resultado.                                                 *
 *                                                                                                   *
 *****************************************************************************************************
 *                                             HOW TO USE                                            *
 * ------------------------------------------------------------------------------------------------- *
 * Compilar :  gcc -o taller_procesos taller_procesos.c                                              *
 * Ejecutar :  ./taller_procesos N1 fichero00 N2 fichero01                                           *
 *                                                                                                   *
 *****************************************************************************************************/
#include "funciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
/************************************************************************************************************/
/*                                                                                                          */
/*  int main(int argc, char *argv[]);                                                                       */
/*                                                                                                          */
/*  Propósito: Crear 3 procesos hijos usando fork():                                                        */
/*             - Grand hijo  : lee fichero00 y envía sumaA por pipeA.                                       */
/*             - Segundo hijo: lee fichero01 y envía sumaB por pipeB.                                       */
/*             - Primer hijo : lee ambos ficheros, calcula sumaT y la envía por pipeT.                      */
/*             El proceso padre recibe por cada pipe y muestra los resultados.                              */
/*                                                                                                          */
/*  Partes del main:                                                                                        */
/*    Parte 1: Validación y parseo de parámetros (N1, fichero00, N2, fichero01) y creación de pipes.        */
/*    Parte 2: fork() del GRAND hijo; lee fichero00 (N1 enteros), calcula sumaA y la escribe en pipeA.      */
/*    Parte 3: fork() del SEGUNDO hijo; lee fichero01 (N2 enteros), calcula sumaB y la escribe en pipeB.    */
/*    Parte 4: fork() del PRIMER hijo; lee ambos ficheros, calcula sumaT = sumaA + sumaB y la escribe en T. */
/*    Parte 5: Proceso PADRE; cierra escritura, lee de pipeA/pipeB/pipeT y muestra rA, rB y rT.             */
/*                                                                                                          */
/*  Parámetros: argc/argv : N1 fichero00 N2 fichero01                                                       */
/*                                                                                                          */
/*  Retorno:   0 en finalización correcta; códigos de error en fallos de sistema.                           */
/*                                                                                                          */
/************************************************************************************************************/
int main(int argc, char *argv[]) {

    /* parte 1 */
    
    /* ---- Validar parámetros: N1 fichero00 N2 fichero01 ---- */
    if (argc != 5) {
        fprintf(stderr, "Uso: %s N1 fichero00 N2 fichero01\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* ---- Parseo de N1/N2 y rutas ---- */
    const unsigned short N1 = (unsigned short)atoi(argv[1]);
    const char *rutaA       = argv[2];

    const unsigned short N2 = (unsigned short)atoi(argv[3]);
    const char *rutaB       = argv[4];

    /* ---- Crear pipes: A, B y Total ---- */
    int pipeA[2], pipeB[2], pipeT[2];
    if (pipe(pipeA) == -1 || pipe(pipeB) == -1 || pipe(pipeT) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    /* parte 2 */

    /* ---- fork del GRAND ---- */
    pid_t pid_grand = fork();
    if (pid_grand < 0) { perror("fork grand"); return EXIT_FAILURE; }

    if (pid_grand == 0) {
        /* ---- Cerrar extremos no usados ---- */
        close(pipeA[0]);
        close(pipeB[0]); close(pipeB[1]);
        close(pipeT[0]); close(pipeT[1]);

        /* ---- Lectura de fichero00 y suma ---- */
        int *A   = leer_enteros((char *)rutaA, N1);
        int sumaA = sumar_arreglo(A, N1);

        /* ---- Enviar sumaA por pipeA ---- */
        int escBytes = write(pipeA[1], &sumaA, (unsigned)sizeof(sumaA));
        if (escBytes == -1) { perror("ESCRITURA"); exit(EXIT_FAILURE); }

        /* ---- Cierre y liberación ---- */
        close(pipeA[1]);
        free(A);
        exit(0);
    }

    /* parte 3 */

    /* ---- fork del SEGUNDO ---- */
    pid_t pid_segundo = fork();
    if (pid_segundo < 0) { perror("fork segundo"); return EXIT_FAILURE; }

    if (pid_segundo == 0) {
        /* ---- Cerrar extremos no usados ---- */
        close(pipeB[0]);
        close(pipeA[0]); close(pipeA[1]);
        close(pipeT[0]); close(pipeT[1]);

        /* ---- Lectura de fichero01 y suma ---- */
        int *B   = leer_enteros((char *)rutaB, N2);
        int sumaB = sumar_arreglo(B, N2);

        /* ---- Enviar sumaB por pipeB ---- */
        int escBytes = write(pipeB[1], &sumaB, (unsigned)sizeof(sumaB));
        if (escBytes == -1) { perror("ESCRITURA"); exit(EXIT_FAILURE); }

        /* ---- Cierre y liberación ---- */
        close(pipeB[1]);
        free(B);
        exit(0);
    }

    /* parte 4 */

    /* ---- fork del PRIMER ---- */
    pid_t pid_primer = fork();
    if (pid_primer < 0) { perror("fork primer"); return EXIT_FAILURE; }

    if (pid_primer == 0) {
        /* ---- Cerrar extremos no usados ---- */
        close(pipeT[0]);
        close(pipeA[0]); close(pipeA[1]);
        close(pipeB[0]); close(pipeB[1]);

        /* ---- Leer ambos ficheros, sumar y componer total ---- */
        int *A    = leer_enteros((char *)rutaA, N1);
        int *B    = leer_enteros((char *)rutaB, N2);
        int sumaA = sumar_arreglo(A, N1);
        int sumaB = sumar_arreglo(B, N2);
        int sumaT = sumaA + sumaB;

        /* ---- Enviar sumaT por pipeT ---- */
        int escBytes = write(pipeT[1], &sumaT, (unsigned)sizeof(sumaT));
        if (escBytes == -1) { perror("ESCRITURA"); exit(EXIT_FAILURE); }

        /* ---- Cierre y liberación ---- */
        close(pipeT[1]);
        free(A);
        free(B);
        exit(0);
    }

    /* parte 5 */

    /* ---- Cerrar extremos de escritura del padre ---- */
    close(pipeA[1]);
    close(pipeB[1]);
    close(pipeT[1]);

    /* ---- Lecturas de resultados ---- */
    int rA = 0, rB = 0, rT = 0;

    /* ---- Leer sumaA ---- */
    int lecBytes = read(pipeA[0], &rA, (unsigned)sizeof(rA));
    (void)lecBytes; /* ---- Ignorado en esta versión ---- */
    close(pipeA[0]);

    /* ---- Leer sumaB ---- */
    lecBytes = read(pipeB[0], &rB, (unsigned)sizeof(rB));
    (void)lecBytes;
    close(pipeB[0]);

    /* ---- Leer sumaT ---- */
    lecBytes = read(pipeT[0], &rT, (unsigned)sizeof(rT));
    (void)lecBytes;
    close(pipeT[0]);

    /* ---- Salida final ---- */
    printf("Resultados (PID padre %ld)\n"       , (long)getpid()      );
    printf("Grand hijo  (PID %ld): sumaA = %d\n", (long)pid_grand  , rA);
    printf("Segundo hijo(PID %ld): sumaB = %d\n", (long)pid_segundo, rB);
    printf("Primer hijo (PID %ld): sumaT = %d\n", (long)pid_primer , rT);

    return 0;
}
