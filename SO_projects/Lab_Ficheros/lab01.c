/**********************************************************************************************************>
/*  Programa:     Directorios y ficheros                                                                   >
/*  Autor:        Carolina Ujueta Ricardo                                                                  >
/*  Fecha:        11 de noviembre de 2025                                                                  >
/*                                                                                                         >
/*  Descripción:                                                                                           >
/*                Este programa se encarga de recibir el nombre de un directorio y a su vez,               >
/*                retorna el tamaño de los ficheros que contiene.                                          >
/*                                                                                                         >
/*  Características principales:                                                                           >
/*   - Uso de libreria stat para la comprobación de diferentes estados de un directorio.                   >
/*+++******************************************************************************************************>

#include <sys/types.h> //Define un conjunto de tipos de datos
#include <sys/stat.h> //Libreria para uso de stat (estado)
#include <sys/mman.h> //Mapeo de memoria
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

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

int main(){
        DIR *d;
        char nomdir[90], nomfich[90]; //Variables que guardan el nombre de directorio y del  fichero
        struct stat datos;
        struct dirent *direc;

        printf ("Introduzca el Nombre de un Directorio: ");
        fgets (nomdir,sizeof(nomdir),stdin); //Guarda el nombre y el tamaño del directorio
        nomdir[strlen(nomdir)-1]='\0'; //Eliminamos el \n del Nombre del Fichero
        if ((d=opendir(nomdir))==NULL){ //If que lanza error si el nombre del directorio no existe o es nulo
                printf ("El directorio no existe\n");
                return -1;
        }
        while ((direc=readdir(d)) !=NULL) { //Mientras que el directorio sea diferente de nulo
                strcpy(nomfich, nomdir); //Copia en nomfich el nomdir
                strcat(nomfich, "/"); //Concatena el nombre del fichero con / (Para simular el directorio)
                strcat(nomfich, direc->d_name ); //Reconstruye las rutas de los archivos
                stat (nomfich, &datos); //Obtiene los datos del archivo
        if (S_ISREG(datos.st_mode)) //Comprueba que el archivo sea un archivo regular
                printf ("Nombre: %s\t| Tamaño: %ld\n", direc->d_name, datos.st_size); //Imprime el nombre y el tamaño
        }//Fin del While
        closedir(d);
}
