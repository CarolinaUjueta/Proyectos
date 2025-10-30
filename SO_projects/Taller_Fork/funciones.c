/************************************************* Headers **************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

/* ---- Prototipos ---- */
int *leer_enteros(char *ruta, size_t n);
int   sumar_arreglo(int *v, size_t n);

/************************************************************************************************************/
/*                                                                                                          */
/*  int *leer_enteros(char *ruta, size_t n);                                                                */
/*                                                                                                          */
/*  Propósito: Abrir un fichero de texto y leer exactamente n enteros, devolviendo un vector dinámico.      */
/*                                                                                                          */
/*  Parámetros: ruta : ruta del fichero a leer.                                                             */
/*              n    : cantidad de enteros esperados en el fichero.                                         */
/*                                                                                                          */
/*  Retorno:   Puntero a int con n elementos reservados y cargados.                                         */
/*             En caso de error, se imprime causa y el proceso termina con EXIT_FAILURE.                    */
/*                                                                                                          */
/************************************************************************************************************/
int *leer_enteros(char *ruta, size_t n) {

    /* ---- Abrir archivo en modo lectura ---- */
    FILE *f = fopen(ruta, "r");
    if (!f) {
        fprintf(stderr, "Error abriendo %s: %s\n", ruta, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* ---- Reservar memoria para n enteros ---- */
    int *v = (int *)malloc(n * sizeof(int));
    if (!v) {
        fprintf(stderr, "Error: sin memoria para %zu enteros.\n", n);
        fclose(f);
        exit(EXIT_FAILURE);
    }

    /* ---- Leer n enteros, validando cada conversión ---- */
    for (size_t i = 0; i < n; ++i) {
        if (fscanf(f, "%d", &v[i]) != 1) {
            fprintf(stderr, "Error: %s no tiene %zu enteros válidos (falló en i=%zu).\n", ruta, n, i);
            free(v);
            fclose(f);
            exit(EXIT_FAILURE);
        }
    }

    /* ---- Cerrar archivo y devolver el vector ---- */
    fclose(f);
    return v;
}

/************************************************************************************************************/
/*                                                                                                          */
/*  int sumar_arreglo(int *v, size_t n);                                                                    */
/*                                                                                                          */
/*  Propósito: Acumular y devolver la suma de n elementos del vector v.                                     */
/*                                                                                                          */
/*  Parámetros: v : puntero al arreglo de enteros.                                                          */
/*              n : número de elementos a sumar.                                                            */
/*                                                                                                          */
/*  Retorno:   Suma de los n enteros.                                                                       */
/*                                                                                                          */
/************************************************************************************************************/
int sumar_arreglo(int *v, size_t n) {

    /* ---- Acumulador ---- */
    int acc = 0;

    /* ---- Recorrido y suma ---- */
    for (size_t i = 0; i < n; ++i) {
        acc += v[i];
    }

    /* ---- Resultado ---- */
    return acc;
}
