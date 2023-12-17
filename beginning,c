#include <stdio.h>
#include <mpi.h>

int main() {
    int num_procs, rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == num_procs - 1) {
        int message = rank;
        MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        int received_message;
        MPI_Recv(&received_message, 1, MPI_INT, num_procs - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Сообщение из последнего процесса под номером: %d\n", received_message);
    }

    MPI_Finalize();
    return 0;
}
