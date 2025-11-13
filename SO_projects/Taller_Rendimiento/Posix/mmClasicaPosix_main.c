/************************************************************************************************************/
/*  PROGRAMA: Multiplicación de Matrices con Paralelismo mediante Hilos POSIX (Pthreads)                    */
/*  AUTOR:  J. Corredor, PhD (documentado por Carolina Ujueta Ricardo, estudiante de Ingeniería de Sistemas)*/
/*  FECHA:  13 Noviembre 2025                                                                               */
/*                                                                                                          */
/*  DESCRIPCIÓN:                                                                                            */
/*  Este programa realiza la multiplicación de dos matrices cuadradas de tamaño N utilizando hilos POSIX.   */
/*  Cada hilo calcula un subconjunto de filas de la matriz resultante, mejorando el desempeño en equipos     */
/*  con múltiples núcleos. El uso de `pthread_mutex_t` garantiza la sincronización y seguridad de acceso     */
/*  cuando varios hilos escriben en la matriz compartida.                                                   */
/*                                                                                                          */
/*  CARACTERÍSTICAS PRINCIPALES:                                                                            */
/*  - Inicialización de matrices con valores aleatorios.                                                    */
/*  - División del trabajo por filas entre los hilos creados.                                               */
/*  - Cálculo concurrente de los resultados parciales en cada hilo.                                         */
/*  - Uso de mutex para evitar condiciones de carrera.                                                      */
/*  - Medición del tiempo total de ejecución con `gettimeofday()`.                                          */
/*  - Permite definir el tamaño de la matriz y el número de hilos desde la línea de comandos.               */
/************************************************************************************************************/

#include "mmClasicaPosix.h"

/*------------------------------------------------------------------------------------------
 *  FUNCIÓN PRINCIPAL
 *-----------------------------------------------------------------------------------------*/
int main(int argc, char *argv[]) {

    if (argc < 3) {
        printf("Uso correcto:\n\t$ ./ejecutable <Tamaño_Matriz> <Número_Hilos>\n");
        exit(0);
    }

    int N = atoi(argv[1]);          // Tamaño de la matriz
    int n_threads = atoi(argv[2]);  // Número de hilos
    pthread_t p[n_threads];         // Arreglo de identificadores de hilos
    pthread_attr_t atrMM;           // Atributos de los hilos

    /* Reservar memoria dinámica para las matrices */
    matrixA = (double *)calloc(N*N, sizeof(double));
    matrixB = (double *)calloc(N*N, sizeof(double));
    matrixC = (double *)calloc(N*N, sizeof(double));

    /* Inicialización de matrices */
    srand(time(NULL));
    iniMatrix(matrixA, matrixB, N);
    impMatrix(matrixA, N);
    impMatrix(matrixB, N);

    /* Inicio de la medición de tiempo */
    InicioMuestra();

    /* Inicialización de mutex y atributos */
    pthread_mutex_init(&MM_mutex, NULL);
    pthread_attr_init(&atrMM);
    pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);

    /* Creación de hilos */
    for (int j = 0; j < n_threads; j++) {
        struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros));
        datos->idH = j;
        datos->nH  = n_threads;
        datos->N   = N;
        pthread_create(&p[j], &atrMM, multiMatrix, (void *)datos);
    }

    /* Espera a que todos los hilos terminen */
    for (int j = 0; j < n_threads; j++)
        pthread_join(p[j], NULL);

    /* Final de la medición de tiempo */
    FinMuestra();

    /* Impresión de la matriz resultado */
    impMatrix(matrixC, N);

    /* Liberación de recursos */
    free(matrixA);
    free(matrixB);
    free(matrixC);
    pthread_attr_destroy(&atrMM);
    pthread_mutex_destroy(&MM_mutex);
    pthread_exit(NULL);

    return 0;
}

/************************************************************************************************************/
/*  USO:                                                                                                    */
/*      $ ./multiMatrix_pthread <Tamaño_Matriz> <Número_Hilos>                                              */
/*                                                                                                          */
/*  EJEMPLO:                                                                                                */
/*      $ ./multiMatrix_pthread 4 2                                                                          */
/*                                                                                                          */
/*  SALIDA (para matrices pequeñas):                                                                        */
/*      - Impresión de las matrices A y B inicializadas aleatoriamente.                                     */
/*      - Cálculo concurrente de la matriz C realizado por varios hilos.                                    */
/*      - Tiempo total de ejecución mostrado en microsegundos.                                              */
/************************************************************************************************************/
