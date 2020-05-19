#include <stdio.h>
#include <stdlib.h>
#include <math.h>



char * calcularIntegral (double k, char *output) {

    //Altura 
    double h=0;
    
    //valores das funções. Representam a altura do trapézio.
    double f1=0;
    double f2=0;

    //x1 e x2 representam os limites do intervalo
    double x1=0;
    double x2=0;

    double resultado=0;

    //Resultado exato: 7853.98163
    for (double i=0; i<100; i=i+k) {
        x1 = i; 
        x2 = i+k;

        f2 = sqrt(100*100 - x2*x2);
        f1 = sqrt(100*100 - x1*x1);

        resultado = resultado + (f1 + f2)*(x2 - x1) / 2;
    }
    printf("Teste: %lf", resultado);
    sprintf(output, "%lf", resultado);
    return output;
}