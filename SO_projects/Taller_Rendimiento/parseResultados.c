/************************************************************************************************************/
/*                                      Pontificia Universidad Javeriana                                    */
/* Autor:      Carolina Ujueta Ricardo                                                                      */
/* Fecha:      14 de Noviembre de 2025                                                                      */
/* Materia:    Sistemas Operativos                                                                          */
/* Profesor:   John Jairo Corredor, PhD                                                                     */
/* Fichero:    parseResultados.c                                                                            */
/* Objetivo:   Leer archivos .dat con múltiples repeticiones de la MM y generar un archivo CSV              */
/*             con (Algoritmo, TamMatriz, Hilos, Repeticion, Tiempo_microsegundos) para Excel.              */
/* HOW TO USE / COMPILE / RUN                                                                               */
/*                                                                                                          */
/*  COMPILAR:                                                                                               */
/*      $ gcc -Wall -Wextra -std=c11 parseResultados.c -o parseResultados                                   */
/*                                                                                                          */
/*  EJECUTAR (ejemplos en carpeta TallerRendimiento):                                                       */
/*      # Solo Fork                                                                                         */
/*      $ ./parseResultados resultados_fork.csv mmClasicaFork-*.dat                                         */
/*                                                                                                          */
/*  SUPOSICIÓN IMPORTANTE SOBRE NOMBRES DE ARCHIVO:                                                         */
/*      Patron:  <Algoritmo>-<TamMatriz>-Hilos-<NumHilos>.dat                                               */
/*      Ejemplo: mmClasicaFork-2-Hilos-1.dat  -> TamMatriz = 2, Hilos = 1                                   */
/************************************************************************************************************/

/***************************************** Headers **********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/***************************************** Prototipos *******************************************************/
int  parse_filename(const char *ruta, char *algoritmo, int *tamMatriz, int *hilos);
void procesar_archivo(const char *ruta, FILE *salida);


/* Firma:     int parse_filename(const char *ruta, char *algoritmo, int *tamMatriz, int *hilos)             */
/* Propósito: Obtener Algoritmo, tamaño de matriz y número de hilos a partir del nombre del archivo .dat.   */
/*            Se asume un patrón tipo: "<algo>-<tam>-Hilos-<hilos>.dat".                                    */
/* Parámetros:                                                                                              */
/*      ruta      -> Ruta completa o nombre del archivo .dat.                                               */
/*      algoritmo -> Buffer para el nombre del algoritmo (ej: "mmClasicaFork").                             */
/*      tamMatriz -> Variable de salida para el tamaño de la matriz (N de NxN).                             */
/*      hilos     -> Variable de salida para el número de hilos.                                            */
/* Retorno:                                                                                                 */
/*      0 si el nombre coincide con el patrón esperado; -1 en caso contrario.                               */

int parse_filename(const char *ruta, char *algoritmo, int *tamMatriz, int *hilos)
{
    const char *base = strrchr(ruta, '/');

    /* ---- Si hay ruta con '/', saltar hasta el nombre base ---- */
    if (base != NULL) {
        base++;           /* saltar el '/' */
    } else {
        base = ruta;
    }

    /* ---- Leer algoritmo, tamaño de matriz y hilos ---- */
    if (sscanf(base, "%63[^-]-%d-Hilos-%d.dat", algoritmo, tamMatriz, hilos) != 3) {
        return -1;
    }

    return 0;
}


/* Firma:     void procesar_archivo(const char *ruta, FILE *salida)                                         */
/* Propósito: Abrir un archivo .dat, extraer todas las apariciones de                                       */
/*            "Tiempo total de ejecución" y escribir una fila en el CSV por cada repetición.                */
/* Parámetros:                                                                                              */
/*      ruta   -> Ruta del archivo .dat a procesar.                                                         */
/*      salida -> Fichero CSV ya abierto en modo escritura.                                                 */


void procesar_archivo(const char *ruta, FILE *salida)
{
    FILE *f;
    char  linea[512];
    char  algoritmo[64];
    int   tamMatriz;
    int   hilos;
    int   repeticion = 0;

    /* ---- Obtener metadatos desde el nombre del archivo ---- */
    if (parse_filename(ruta, algoritmo, &tamMatriz, &hilos) != 0) {
        fprintf(stderr,
                "Advertencia: nombre no coincide con el patrón esperado: '%s'\n",
                ruta);
        return;
    }

    /* ---- Abrir el archivo .dat ---- */
    f = fopen(ruta, "r");
    if (f == NULL) {
        perror(ruta);
        return;
    }

    /* ---- Recorrer todas las líneas buscando "Tiempo total de ejecución" ---- */
    while (fgets(linea, sizeof(linea), f) != NULL) {
        if (strstr(linea, "Tiempo total de ejecución") != NULL) {
            char *p = linea;
            long  tiempo;

            /* ---- Avanzar hasta el primer dígito de la línea ---- */
            while (*p != '\0' && !isdigit((unsigned char) *p)) {
                p++;
            }

            /* ---- Si se encontró un número, convertirlo ---- */
            if (*p != '\0') {
                tiempo = strtol(p, NULL, 10);
                repeticion++;

                /* ---- Escribir una fila en el CSV: una por repetición ---- */
                fprintf(salida, "%s;%d;%d;%d;%ld\n",
                        algoritmo, tamMatriz, hilos, repeticion, tiempo);
            }
        }
    }

    fclose(f);
}


/* Firma:     int main(int argc, char *argv[])                                                              */
/* Propósito: Procesar todos los archivos .dat pasados por línea de comandos y generar un CSV con           */
/*            una fila por cada "Tiempo total de ejecución" encontrado.                                     */
/* Parámetros:                                                                                              */
/*      argc / argv -> Argumentos de línea de comandos.                                                     */
/* Retorno:                                                                                                 */
/*      0 si todo fue correcto, distinto de 0 si ocurrió algún error grave.                                 */

int main(int argc, char *argv[])
{
    FILE *salida;
    int   i;

    /* ---- Validar cantidad de argumentos ---- */
    if (argc < 3) {
        fprintf(stderr,
                "USO: %s salida.csv archivo1.dat archivo2.dat ...\n"
                "Ejemplo:\n"
                "   %s resultados.csv mmClasicaFork-*.dat\n",
                argv[0], argv[0]);
        return 1;
    }

    /* ---- Abrir el archivo CSV de salida ---- */
    salida = fopen(argv[1], "w");
    if (salida == NULL) {
        perror("No se pudo crear el archivo CSV de salida");
        return 1;
    }

    /* ---- Escribir la cabecera del CSV ---- */
    fprintf(salida,
            "Algoritmo;TamMatriz;Hilos;Repeticion;Tiempo_microsegundos\n");

    /* ---- Procesar cada .dat pasado como argumento ---- */
    for (i = 2; i < argc; i++) {
        procesar_archivo(argv[i], salida);
    }

    /* ---- Cerrar archivo de salida ---- */
    fclose(salida);

    return 0;
}
