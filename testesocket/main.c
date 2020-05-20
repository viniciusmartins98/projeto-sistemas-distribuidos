#include <stdlib.h>
#include <stdio.h>
#include "partial_calculator.h"

int main() {
    double resultado;
    resultado = resolveIntegral(0.0001, 0, 25);
    printf("%lf\n", resultado);
    resultado = resultado + resolveIntegral(0.0001, 25, 50);
    printf("%lf\n", resultado);
    resultado = resultado + resolveIntegral(0.0001, 50, 75);
    printf("%lf\n", resultado);
    resultado = resultado + resolveIntegral(0.0001, 75, 100);
    printf("%lf\n", resultado);

    return 0;
}
