#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

/*


Generate exect : mpicc -g -Wall -o matriz_vector_pmi matriz_vector_pmi.c
run exect:mpiexec -n 4 ./matriz_vector_pmi 

*/

int **A;
int *x;
int *y;
int dimension = 4;
double data;
void create_arrays();

void process(int my_rank, int size);

int main(void)
{

    int size;
    int my_rank;

    // manda parametros del main
    MPI_Init(NULL, NULL);

    //retorna cantidad de procesos que estan ejecutandose
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // ID del proceso que esta ejecutando esa linea
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    process(my_rank, size);

    MPI_Finalize();

    return 0;
}

void create_arrays()
{
    printf("Creating array data\n");
    A = (int **)malloc(dimension * sizeof(int *));
    for (int i = 0; i < dimension; i++)
    {
        A[i] = (int *)malloc(dimension * sizeof(int));
    }

    x = malloc(dimension * sizeof(int));
    y = malloc(dimension * sizeof(double));

    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            A[i][j] = rand() % 50;
            printf("A[%d][%d] = %d\n", i, j, A[i][j]);
        }
        x[i] = rand() % 50;
        printf("x[%d] = %d\n", i, x[i]);
    }

    printf("-----------------------------*-------------------------\n");
}

void multiply()
{
    for (int i = 0; i < dimension; i++)
    {
        data = 0;
        for (int j = 0; j < dimension; j++)
        {
            data += A[i][j] * x[j];
        }
        MPI_Send(&data, 8, MPI_DOUBLE, i + 1, 0, MPI_COMM_WORLD); //el cero se los manda a los demas
    }
}

void process(int my_rank, int size)
{

    if (my_rank == 0)
    {
        create_arrays();
        multiply();
    }
    else
    {

        MPI_Recv(&data, 8, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // los demas lo reciben del cero
        printf("result=%lf\n", data);
    }
}