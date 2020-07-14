#include <stdio.h>
#include <mpi.h>
#include "utils.h"
#define maxtime 100000

int main(int argc, char **argv) {
    
    double resultado_parcial = 0;
    double resultado_final = 0; 
    //int s, e, ls, le, mylen, i, r, c, tick;
    //int i, tick;
    int id_proc, total_proc, id_slave, mestre, tag;
    int interval;
    //MPI_Status status;
    MPI_Init (&argc, &argv);

    // 1) Pegar quantos processos irão ser executados

    // 2) A partir disso definir o que cada processo ira calcular

    MPI_Comm_size (MPI_COMM_WORLD, &total_proc); // Pega o número de processos que irão ser executados
    MPI_Comm_rank (MPI_COMM_WORLD, &id_proc); // Pega o id do processo
    printf("Processo: %d\n", id_proc);

    // Inicia a execução das iterações
    //for (tick = 1; tick <= maxtime; tick++) {
        // Faz o broadcast da matriz aos demais processos
        //MPI_Bcast(black, (rows+2)*(cols+2), MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // Processo mestre recolhe os resultados na matriz BLACK
        if (id_proc != 0) { 
            mestre = 0;
            tag = 0;
            interval = calculate_interval(total_proc - 1, 0, 100);
            resultado_parcial = resolveIntegral(0.0001, id_proc, interval);
            MPI_Send(&resultado_parcial, sizeof(double), MPI_DOUBLE, mestre, tag, MPI_COMM_WORLD);
            printf("Resultado parcial = %lf\n", resultado_parcial);
        }
        MPI_Reduce(&resultado_parcial, &resultado_final, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        if (id_proc == 0) { 
            // for (i=1; i<=total_proc-1; i++) { 
            //     // printf("Escravo %d\n", i);
            //     // id_slave = i; 
            //     // tag = 0;
            //     // // MPI_Recv(&resultado_parcial, sizeof(double), MPI_DOUBLE, id_slave, tag, MPI_COMM_WORLD, &status);
            //     // printf("Resultado parcial = %lf\n", resultado_parcial);
            //     // // resultado_final = resultado_final + resultado_parcial; 
            // }
            printf("Resultado final = %lf\n", resultado_final);
        } 
    //} 

    MPI_Finalize();
}