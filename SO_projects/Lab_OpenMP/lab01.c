#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
        printf("OpenMP ejecutando con %d hilos\n", omp_get_max_threads());
#pragma omp parallel
{
        printf("Hello Word desde el thread %d\n", omp_get_thread_num());
}
        return 0;
}
