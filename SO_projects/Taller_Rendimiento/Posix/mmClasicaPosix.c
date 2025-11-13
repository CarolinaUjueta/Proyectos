/************************************************************************************************************/
/*  ARCHIVO: mmClasicaPosix.c                                                                               */
/*  AUTOR:  J. Corredor, PhD (documentado por Carolina Ujueta Ricardo, estudiante de Ingeniería de Sistemas)*/
/*  FECHA:  13 Noviembre 2025                                                                               */
/*                                                                                                          */
/*  DESCRIPCIÓN:                                                                                            */
/*  Implementación de las funciones auxiliares utilizadas en la multiplicación de matrices mediante         */
/*  hilos POSIX (Pthreads). Incluye la inicialización de matrices, su impresión, el cálculo de la           */
/*  multiplicación y la medición del tiempo de ejecución.                                                   */
/************************************************************************************************************/

#include "mmClasicaPosix.h"

/*------------------------------------------------------------------------------------------
 *  FUNCIONES DE MEDICIÓN DE TIEMPO
 *-----------------------------------------------------------------------------------------*/

void InicioMuestra() {
    gettimeofday(&inicio, (void *)0);
}

void FinMuestra() {
    gettimeofday(&fin, (void *)0);
    fin.tv_usec -= inicio.tv_usec;
    fin.tv_sec  -= inicio.tv_sec;
    double tiempo = (double)(fin.tv_sec * 1000000 + fin.tv_usec);
    printf("\nTiempo total de ejecución: %.6f segundos\n", tiempo / 1e6);
}

/*------------------------------------------------------------------------------------------
 *  FUNCIÓN: iniMatrix()
 *  DESCRIPCIÓN:
 *      Inicializa las matrices A y B con valores aleatorios.
 *-----------------------------------------------------------------------------------------*/
void iniMatrix(double *m1, double *m2, int D) {
    for (int i = 0; i < D * D; i++, m1++, m2++) {
        *m1 = (double)rand() / RAND_MAX * (5.0 - 1.0);
        *m2 = (double)rand() / RAND_MAX * (9.0 - 5.0);
    }
}

/*------------------------------------------------------------------------------------------
 *  FUNCIÓN: impMatrix()
 *  DESCRIPCIÓN:
 *      Imprime una matriz en consola si su tamaño es menor a 9x9 para evitar saturar salida.
 *-----------------------------------------------------------------------------------------*/
void impMatrix(double *matriz, int D) {
    if (D < 9) {
        for (int i = 0; i < D * D; i++) {
            if (i % D == 0)
                printf("\n");
            printf(" %.2f ", matriz[i]);
        }
        printf("\n>-------------------->\n");
    }
}

/*------------------------------------------------------------------------------------------
 *  FUNCIÓN: multiMatrix()
 *  DESCRIPCIÓN:
 *      Ejecutada por cada hilo. Calcula las filas asignadas de la matriz resultante C.
 *      Cada hilo toma un bloque de filas de A y las multiplica por B.
 *-----------------------------------------------------------------------------------------*/
void *multiMatrix(void *variables) {
    struct parametros *data = (struct parametros *)variables;

    int idH  = data->idH;
    int nH   = data->nH;
    int D    = data->N;
    int filaI = (D / nH) * idH;
    int filaF = (D / nH) * (idH + 1);

    double Suma, *pA, *pB;

    for (int i = filaI; i < filaF; i++) {
        for (int j = 0; j < D; j++) {
            pA = matrixA + i * D;
            pB = matrixB + j;
            Suma = 0.0;
            for (int k = 0; k < D; k++, pA++, pB += D) {
                Suma += *pA * *pB;
            }
            matrixC[i * D + j] = Suma;
        }
    }

    pthread_mutex_lock(&MM_mutex);
    pthread_mutex_unlock(&MM_mutex);
    pthread_exit(NULL);
}

/************************************************************************************************************/
/*  FIN DEL ARCHIVO: mmClasicaPosix.c                                                                       */
/************************************************************************************************************/
