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
    MPI_Init (&argc, &argv);

    MPI_Comm_size (MPI_COMM_WORLD, &total_proc); // Pega o número de processos que irão ser executados
    MPI_Comm_rank (MPI_COMM_WORLD, &id_proc); // Pega o id do processo

    // *** Todos as operações entre os dois MPI_Wtime serão calculadas em segundos.
    // Pega o valor do tempo inicial
    tempo_inicial = MPI_Wtime(); 

    // Caso o processo não seja o mestre, ou seja todos os escravos
    if (id_proc != 0) { 
        mestre = 0; // Id do mestre é 0
        tag = 0; //
        interval = calculate_interval(total_proc - 1, 0, 100); // Calcula o intervalo da integral que cada escravos irá calcular
        resultado_parcial = resolveIntegral(k, id_proc, interval); // O escravo de identificador "id_proc" calcula o seu resultado parcial dentro do seu intervalo
        MPI_Send(&resultado_parcial, sizeof(double), MPI_DOUBLE, mestre, tag, MPI_COMM_WORLD); // Escravo envia o resultado para o mestre
        printf("(ESCRAVO %d) Resultado parcial = %lf\n", id_proc, resultado_parcial); //É exibido o intervalo parcial calculado pelo escravo
    }

    /* int MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)
        sendbuf
            endereço do buffer que será enviado
        recvbuf
            endereço em que será acumulado os valores parciais
        count
            numero de elementos que serão enviados
        datatype
            tipo de dados dos elementos que serão enviados
        op
            operação que será realizado, no caso é um somatório (MPI_SUM)
        root
            id do processo mestre
        comm
            comunicador, guarda as informações do MPI

    O MPI_Reduce recebe como entrada um vetor de elementos para cada processo e retorna um vetor de elementos na saída
    para o mestre. Os elementos de saída contêm o resultado reduzido, que no caso representa o somatório de todos os
    resultados parciais calculados pelos escravos.*/
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