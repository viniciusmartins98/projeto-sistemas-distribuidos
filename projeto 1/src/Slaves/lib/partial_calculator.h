#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double resolveIntegral (double k, double init, double final) {
    
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

    //De 0 a 50;
    double i;
    for (i = init; i < final - k; i = i + k) {
        x1 = i;
        x2 = x1 + k;

        f2 = sqrt(100*100 - x2*x2);
        f1 = sqrt(100*100 - x1*x1);

        resultado = resultado + (f1 + f2)*(x2 - x1) / 2;
    }

    return resultado;
}