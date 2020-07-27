#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "utils.h"

int main(int argc, char **argv) {
    
    double resultado_parcial = 0;
    double resultado_final = 0; 
    double tempo_inicial = 0;
    double tempo_final = 0;
    double k = atof(argv[1]);
    int id_proc, total_proc, id_slave, mestre, tag;
    int interval;
    //MPI_Status status;
    MPI_Init (&argc, &argv);

    MPI_Comm_size (MPI_COMM_WORLD, &total_proc); // Pega o número de processos que irão ser executados
    MPI_Comm_rank (MPI_COMM_WORLD, &id_proc); // Pega o id do processo

    // *** Todos as operações entre os dois MPI_Wtime serão calculadas em segundos.
    // Pega o valor do tempo inicial
    tempo_inicial = MPI_Wtime(); 

    // Caso o processo não seja o mestre, ou seja todos os slaves
    if (id_proc != 0) { 
        mestre = 0; // Id do mestre é 0
        tag = 0; //
        interval = calculate_interval(total_proc - 1, 0, 100); // Calcula o intervalo da integral que cada slave irá calcular
        resultado_parcial = resolveIntegral(k, id_proc, interval); // O slave de identificador "id_proc" calcula o seu resultado parcial dentro do seu intervalo
        MPI_Send(&resultado_parcial, sizeof(double), MPI_DOUBLE, mestre, tag, MPI_COMM_WORLD); // Slave envia o resultado para o mestre
        printf("(SLAVE %d) Resultado parcial = %lf\n", id_proc, resultado_parcial); //É exibido o intervalo parcial calculado pelo slave
    }

    MPI_Reduce(&resultado_parcial, &resultado_final, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Pega o valor do tempo final
    tempo_final = MPI_Wtime();
    if (id_proc == 0) { 
        // Calcula o tempo final
        printf("(MESTRE) Resultado final = %lf\n", resultado_final);
        printf("(MESTRE) Tempo gasto = %lf\n", tempo_final - tempo_inicial);
    } 

    MPI_Finalize();
}