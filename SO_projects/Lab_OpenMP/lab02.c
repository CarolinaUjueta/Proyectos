/*      Autor: Carolina Ujueta Ricardo
        Progrma:
        Fecha: 4 noviembre 2025
        Tema:
objetivo : introduccion en laboratorio openp
           clausulas y directivas para el programa
          gcc -fopenmp taller1.c -o taller1
*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]){
        if(argc != 2){
                printf( "ERROR\n\t $./ejecutable numHilos\n");
                exit(0);
        }

        int numHilos = (int) atoi(argv[1]);
        int maxHilos = omp_get_max_threads();
        printf("OpenMP ejecutando en cores =  %d hilos \n", omp_get_max_threads());
        omp_set_num_threads(numHilos);

        #pragma omp parallel
        {
        printf("Hello world desde el thread %d\n", omp_get_thread_num());
        }
        return 0;
}
