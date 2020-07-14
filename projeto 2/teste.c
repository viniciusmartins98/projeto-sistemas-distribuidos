#include "stdio.h"
#include <mpi.h>
#define MINPROC 4
#define cols 200
#define rows 200
#define maxtime 100000

int main(int argc, char **argv) {
    double red[rows+2][cols+2], black[rows+2][cols+2];
    int s, e, ls, le, mylen, i, r, c, tick;
    int inum, nproc, src, dest, tag;
    MPI_Status status;
    puts ("Chamando MPI_INIT");
    MPI_Init (&argc, &argv);
    MPI_Comm_size (MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank (MPI_COMM_WORLD, &inum);
    s = 1 + inum*(rows/nproc);
    e = s + (rows/nproc) – 1;

    // Começa frio (todos os pontos em 0◦C)
    if (inum==0)
        for (r=0; r <= mylen+1; r++)
            for (c=0; c <= cols+1; c++)
                black[r][c] = 0.0;
    // Inicia a execução das iterações
    for (tick = 1; tick <= maxtime; tick++) {

        // Inicia valores das fontes constantes de calor
        black[rows/3][cols/3] = 10.0;
        black[2*rows/3][cols/3] = 20.0;
        black[rows/3][2*cols/3] = -20.0;
        black[2*rows/3][2*cols/3] = 20.0;

        // Faz o broadcast da matriz aos demais processos
        MPI_Bcast(black, (rows+2)*(cols+2), MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // Calcula uma iteração do fluxo para sua parte da matriz
        for (r=s; r<=e; r++)
            for (c=1; c<=cols; c++)
                red[r][c] = ( black[r][c] +
                    black[r][c-1] +
                    black[r-1][c] +
                    black[r+1][c] +
                    black[r][c+1] )
                        / 5.0;
        // Processo mestre recolhe os resultados na matriz BLACK
        if (inum == 0) { 
            for (r=s; r<=e; r++)
                for (c=1; c<=rows; c++)
                    black[r][c] = red[r][c];
            for (i=1; i<=nproc-1; i++) { 
                ls = 1 + i * (rows/nproc);
                le = ls + (rows/nproc) - 1;
                mylen = (le - ls) + 1; src = i; tag = 0;
                MPI_Recv(&black[ls][0], mylen*(cols+2), MPI_DOUBLE, src, tag, MPI_COMM_WORLD, &status);
            }
        } // final do "then" para "if (inum==0)" ;
        else { 
            mylen = (e -s) + 1;
            dest = 0;
            tag = 0;
            MPI_Send (&red[s][0], mylen*(cols+2), MPI_DOUBLE,
            dest, tag, MPI_COMM_WORLD);
        }
    } /* Final do laço principal "for (tick = 1; tick <=
    maxtime; tick++)" */
    if (inum==0) { 
        for (r=62; r <= 72; r++) {
            for (c=62; c <= 70; c++) {
                printf("%lf ",black[r][c]);
                puts(" "); 
            }
        }
    }

    MPI_Finalize();
}