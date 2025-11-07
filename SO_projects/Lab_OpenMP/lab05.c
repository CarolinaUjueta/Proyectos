#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define Min(x,y) (((x) < (y))? (x): (y))

double funcion(int i){
        int  j, inicio=i*(i+1)/2, final=inicio+1;
        double  valor= 0 ;
        for  (int j=0; j<inicio; j++)
                valor += sin(j);
        return valor;
}

int main(){
        double sum = 0;
        int i, n = 30000;

        #pragma omp parallel
{
        int threads = omp_get_num_threads();
        #pragma omp master
        printf("Numero de Hilos: %d \n", threads);
        #pragma omp for reduction(+ :sum)
        for (i = 0; i<= n; i++){
                sum +=funcion(i);
}
}
        printf("La sumatoria es %.2f\n", sum);
        return 0;
}
