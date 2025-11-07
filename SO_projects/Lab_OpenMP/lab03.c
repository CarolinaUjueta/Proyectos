#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
        int i;
        const int N = 1000;
        int a = 50;
        int b = 0;

        #pragma omp parallel for private(i) private(a)
        for (i=0; i<N; i++){
                b = a + i;
        }
        printf("a = %d b = %d (Se espera a=50 b=1049)\n", a,b);
        return 0;
}

