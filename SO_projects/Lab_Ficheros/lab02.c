/************************************************************************************************************/
/*  Programa:     Directorios y ficheros                                                                    */
/*  Autor:        Carolina Ujueta Ricardo                                                                   */
/*  Fecha:        11 de noviembre de 2025                                                                   */
/*                                                                                                          */
/*  Descripción:                                                                                            */
/*                Este programa solicita al usuario el nombre de un directorio y, a partir de él,           */
/*                recorre su contenido mostrando los ficheros que contiene. Además, verifica los permisos   */
/*                de lectura del propietario, identifica si los elementos son directorios o ficheros,       */
/*                e imprime información adicional sobre los archivos modificados en los últimos 10 días.    */
/*                                                                                                          */
/*  Características principales:                                                                            */
/*   - Uso de la librería <stat.h> para obtener atributos de archivos.                                      */
/*   - Uso de la librería <dirent.h> para el manejo de directorios.                                         */
/*   - Empleo de estructuras para obtener permisos y fechas de modificación.                                */
/************************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>     // Interfaz para manejo de directorios 
#include <fcntl.h>      // Control de archivos (modos de apertura)
#include <unistd.h>
#include <sys/stat.h>   // Estructura stat y macros para información de archivos
#include <sys/types.h>  // Definiciones de tipos de datos para el sistema de archivos

/************************************************************************************************************/
/*  int main()                                                                                              */
/*                                                                                                          */
/*  Propósito:                                                                                              */
/*      Solicita el nombre de un directorio e imprime información sobre sus ficheros,                       */
/*      incluyendo permisos, tipo de archivo y fecha de última modificación.                                */
/*                                                                                                          */
/*  Retorno:                                                                                                */
/*      - Retorna -1 si no se encuentra o no se puede abrir el directorio.                                  */
/*      - Si el directorio existe, muestra información sobre los ficheros que contiene.                     */
/************************************************************************************************************/

int main() {
    int er;                          // Variable para almacenar el resultado de la función stat
    char nomdir[100], nomfich[200];  // Arrays para el nombre del directorio y del fichero completo
    char resp[30];                   // Variable no utilizada, reservada para futuras mejoras
    struct stat atr;                 // Estructura para almacenar atributos del archivo
    DIR *d;                          // Puntero al directorio que se abrirá
    struct dirent *rd1;              // Estructura que almacena la información de cada entrada del directorio
    time_t fecha;                    // Variable para manejar la fecha y hora actuales

    printf("Nombre directorio: ");
    fgets(nomdir, sizeof(nomdir), stdin);     // Lee el nombre del directorio introducido por el usuario
    nomdir[strlen(nomdir) - 1] = '\0';        // Elimina el salto de línea '\n' al final del nombre
    fecha = time(&fecha);                     // Guarda la fecha y hora actuales del sistema

    // Intenta abrir el directorio especificado por el usuario
    if ((d = opendir(nomdir)) == NULL) {
        printf("El directorio no existe\n");  // Mensaje de error si no se puede abrir
        return -1;                            // Retorna -1 indicando fallo
    } else {
        // Recorre las entradas del directorio una por una
        while ((rd1 = readdir(d)) != NULL) {
            // Ignora las entradas "." y ".."
            if ((strcmp(rd1->d_name, ".") != 0) && (strcmp(rd1->d_name, "..") != 0)) {
                
                // Construye la ruta completa del archivo
                strcpy(nomfich, nomdir);
                strcat(nomfich, "/");
                strcat(nomfich, rd1->d_name);

                printf("fichero :%s:", nomfich);  // Muestra la ruta completa del archivo
                
                er = stat(nomfich, &atr);         // Obtiene los atributos del archivo
                printf("modo :%#o:", atr.st_mode); // Muestra los permisos en formato octal

                // Comprueba si el propietario tiene permiso de lectura
                if ((atr.st_mode & 0400) != 0)
                    printf(" permiso R para propietario\n");
                else
                    printf(" No permiso R para propietario\n");
            } else {
                // Segundo recorrido alternativo dentro del mismo directorio
                while ((rd1 = readdir(d)) != NULL) {
                    if ((strcmp(rd1->d_name, ".") != 0) && (strcmp(rd1->d_name, "..") != 0)) {
                        
                        // Construye nuevamente la ruta completa del archivo
                        strcpy(nomfich, nomdir);
                        strcat(nomfich, "/");
                        strcat(nomfich, rd1->d_name);
                        printf("fichero :%s:\n", nomfich);

                        er = stat(nomfich, &atr);   // Obtiene los datos del archivo
                        printf("modo :%#o:\n", atr.st_mode);

                        // Verifica el permiso de lectura para el propietario
                        if ((atr.st_mode & 0400) != 0)
                            printf("permiso R para propietario\n");
                        else
                            printf("No permiso R para propietario\n");

                        // Determina si es un directorio
                        if (S_ISDIR(atr.st_mode)) {
                            printf("Es un directorio\n");
                        }

                        // Determina si es un archivo regular
                        if (S_ISREG(atr.st_mode)) {
                            /* Ficheros modificados en los últimos 10 días */
                            if (((fecha) - 10 * 24 * 60 * 60) < atr.st_mtime) {
                                printf("FICHERO:%s: fecha acceso %s, en sgdos %ld\n",
                                       rd1->d_name,
                                       ctime(&atr.st_mtime),
                                       atr.st_mtime);
                            }
                        }
                    }
                } /* while interno */
                closedir(d); // Cierra el directorio después de procesarlo
            }
        }
    }
}/* main */

/************************************************************************************************************/
/*  Conclusiones:                                                                                           */
/*                                                                                                          */
/*      Este laboratorio permite el entendimiento de los directorios y ficheros.                            */                                      */
/*      Nos permite una mejor organización y separación de responsabilidades.                               */
/*      En este caso, podemos gestionar los propietarios, la última fecha de modificación e identifica si   */
/*      son ficheros o directorios.                                                                         */
/*      También nos ayuda a aclarar el funcionamiento de la memoria no volátil, ya que se conservan los     */
/*      datos en memoria, a pesar de que se apague el sistema.                                              */
/*                                                                                                          */
/************************************************************************************************************/
