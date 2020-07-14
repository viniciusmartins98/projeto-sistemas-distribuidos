#include <stdio.h>
#include <mpi.h>
#include "utils.h"

int main(int argc, char **argv) {
    double tempo_inicial = 0;
    double tempo_final = 0;
    double tempo_gasto = 0;

    double resultado_parcial = 0;
    double resultado_final = 0; 
    int id_proc, total_proc, id_slave, mestre, tag;
    int interval;

    MPI_Init (&argc, &argv);

    MPI_Comm_size (MPI_COMM_WORLD, &total_proc); // Pega o número de processos que irão ser executados
    MPI_Comm_rank (MPI_COMM_WORLD, &id_proc); // Pega o id do processo
    printf("Processo: %d\n", id_proc);

    if (id_proc != 0) { 
        mestre = 0;
        tag = 0;
        interval = calculate_interval(total_proc - 1, 0, 100);
        resultado_parcial = resolveIntegral(0.000001, id_proc, interval);
        MPI_Send(&resultado_parcial, sizeof(double), MPI_DOUBLE, mestre, tag, MPI_COMM_WORLD);
        printf("Resultado parcial = %lf\n", resultado_parcial);
    }
    MPI_Reduce(&resultado_parcial, &resultado_final, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (id_proc == 0) { 
        // Resultado final da integral
        printf("Resultado final = %lf\n", resultado_final);
    }  

    MPI_Finalize();
}