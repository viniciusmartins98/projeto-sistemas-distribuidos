#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Intervalo de discretização.
#define k1 0.0001
#define k2 0.00001
#define k3 0.000001

int main (char args[]) {
    //Intervalo final
    double B=0;
    //Intervalo inicial
    double b=0;

    //Altura 
    double h=0;
    double f1=0;
    double f2=0;

    double x1=0;
    double x2=0;

    double resultado=0;


    //(B + b)h/2

    //B = f(x1)
    //b = f(x2)
    //h = x2 - x1
    //f(x) = sqrt(100² - x²);

    //Resultado exato: 7853.98163

    for (double i=k1; i<=100; i=i+k1) {
        x1 = (i-k1); 
        x2 = i;

        f2 = sqrt(100*100 - x2*x2);
        f1 = sqrt(100*100 - x1*x1);

        resultado = resultado + (f1 + f2)*(x2 - x1) / 2;
    }

    printf("Integral: %f\n", resultado);

    return 0;
}