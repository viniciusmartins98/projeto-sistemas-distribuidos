#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Intervalo de discretização.
#define k1 0.0001
#define k2 0.00001
#define k3 0.000001

int main (char args[]) {
    float a=0;
    float b=100;
    float n=0;
    float h=0;
    float fx=0;
    float soma=0;
    float result=0;

    for (double i = a; i <= b; i = i + k3)
    {
        n = (b - a) / k3;
        h = ( (b + k3) - a ) / n;

        fx = sqrt(100*100 - i*i);

        if (i == a || i == (b + k3))
        {
            soma = soma + fx;
        }
        else
        {
            soma = soma + 2 * fx;
        }

        result = soma * h / 2;
    }

    printf("Integral: %f\n", result);

    return 0;
}