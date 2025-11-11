/************************************************************************************************************/
/*  Programa:     Directorios y ficheros                                                                    */
/*  Autor:        Carolina Ujueta Ricardo                                                                   */
/*  Fecha:        11 de noviembre de 2025                                                                   */
/*                                                                                                          */
/*  Descripción:                                                                                            */
/*                Este programa se encarga de recibir el nombre de un directorio y a su vez,                */
/*                retorna el tamaño de los ficheros que contiene.                                           */
/*                                                                                                          */
/*  Características principales:                                                                            */
/*   - Uso de libreria stat para la comprobación de diferentes estados de un directorio.                    */
/************************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

/************************************************************************************************************/
/*  int main()                                                                                              */
/*                                                                                                          */
/*  Propósito:                                                                                              */
/*      Imprime el tamaño de los datos que se encuentran en un directorio.                                  */
/*                                                                                                          */
/*  Retorno:                                                                                                */
/*      - Retorna -1 si no se encuentra el directorio.                                                      */
/*      - Si hay directorio, imprime el tamaño de los ficheros.                                             */
/************************************************************************************************************/

int main() {
    int er;
    char nomdir[100], nomfich[200], resp[30];
    struct stat atr;
    DIR *d;
    struct dirent *rd1;
    time_t fecha;

    printf("Nombre directorio: ");
    fgets(nomdir, sizeof(nomdir), stdin); // Guarda el nombre
    nomdir[strlen(nomdir) - 1] = '\0';    // Elimina el \n del nombre
    fecha = time(&fecha);

    if ((d = opendir(nomdir)) == NULL) { // Si no se puede abrir el directorio
        printf("El directorio no existe\n");
        return -1;
    } else {
        while ((rd1 = readdir(d)) != NULL) {
            if ((strcmp(rd1->d_name, ".") != 0) && (strcmp(rd1->d_name, "..") != 0)) {
                strcpy(nomfich, nomdir);
                strcat(nomfich, "/");
                strcat(nomfich, rd1->d_name);
                printf("fichero :%s:", nomfich);

                er = stat(nomfich, &atr);
                printf("modo :%#o:", atr.st_mode);

                if ((atr.st_mode & 0400) != 0)
                    printf(" permiso R para propietario\n");
                else
                    printf(" No permiso R para propietario\n");
            } else {
                while ((rd1 = readdir(d)) != NULL) {
                    if ((strcmp(rd1->d_name, ".") != 0) && (strcmp(rd1->d_name, "..") != 0)) {
                        strcpy(nomfich, nomdir);
                        strcat(nomfich, "/");
                        strcat(nomfich, rd1->d_name);
                        printf("fichero :%s:\n", nomfich);

                        er = stat(nomfich, &atr);
                        printf("modo :%#o:\n", atr.st_mode);

                        if ((atr.st_mode & 0400) != 0)
                            printf("permiso R para propietario\n");
                        else
                            printf("No permiso R para propietario\n");

                        if (S_ISDIR(atr.st_mode)) {
                            printf("Es un directorio\n");
                        }

                        if (S_ISREG(atr.st_mode)) {
                            /* ficheros modificados en los últimos 10 días */
                            if (((fecha) - 10 * 24 * 60 * 60) < atr.st_mtime) {
                                printf("FICHERO:%s: fecha acceso %s, en sgdos %ld\n",
                                       rd1->d_name,
                                       ctime(&atr.st_mtime),
                                       atr.st_mtime);
                            }
                        }
                    }
                } /* while */
                closedir(d);
            }
        }
    }
}/* main */
