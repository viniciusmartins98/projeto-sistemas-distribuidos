#include <stdio.h>
#include <math.h>
#include <time.h>

#define error -1

// verifies if is executing on windows or linux OS.
#ifdef WIN32

#include <windows.h>

// Função para calcular tempo no windows
double get_time() {
    LARGE_INTEGER t, f;
    QueryPerformanceCounter(&t);
    QueryPerformanceFrequency(&f);
    return (double)t.QuadPart/(double)f.QuadPart;
}

#else

#include <sys/time.h>
#include <sys/resource.h>

// Função para calcular tempo no linux
double get_time() {
    struct timeval t;
    struct timezone tzp;
    gettimeofday(&t, &tzp);
    return t.tv_sec + t.tv_usec*1e-6;
}
#endif

double resolveIntegral (double k, int num_proc, double interval) {
    double f1=0;
    double f2=0;

    double x1=0;
    double x2=0;

    double resultado=0;

    double init, final;
    init = (num_proc - 1) * interval;
    final = init + interval;

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

// calculates size of interval which is going to be divided for servers
int calculate_interval(int num_slave, int init, int final){
    int interval = final - init;
    return interval/num_slave;
}