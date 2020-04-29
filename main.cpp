#include <iostream>
#include <string>
#include <mpi.h>

void MPI_Round() {

	int rank, total;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &total);
	MPI_Status status[2];
	int value[10], input[10];

	if (rank == 0) {
		for (int i = 0; i < 10; ++i) {
			value[i] = i;
		}
		MPI_Send(&value, 10, MPI_INT, (rank + 1) % total, 1, MPI_COMM_WORLD);
	}
	MPI_Recv(&input, 10, MPI_INT, (total + rank - 1) % total, 1, MPI_COMM_WORLD, &status[0]);

	if (rank != 0) {
		MPI_Send(&input, 10, MPI_INT, (rank + 1) % total, 1, MPI_COMM_WORLD);
	}
	std::cout << "rank " << rank << " recived: " ;
	for (int i = 0; i < 10; ++i) {
		std::cout << input[i]<<"\t";
	}
	std::cout << std::endl;


}

void MPI_Shift() {


	int rank, total;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &total);
	MPI_Request request[2];
	MPI_Status status[2];
	int value[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int input[10];

	if (rank == 0) {
		MPI_Isend(&value, 10, MPI_INT, (rank + 1) % total, 1, MPI_COMM_WORLD, &request[0]);
		MPI_Wait(&request[0], &status[0]);
	}

	MPI_Irecv(&input, 10, MPI_INT, (total + rank - 1) % total, 1, MPI_COMM_WORLD, &request[0]);
	MPI_Wait(&request[0], &status[0]);

	if (rank != 0) {
		for (int i = 0; i < 10; ++i) {
			input[i] = i*rank;
		}
		MPI_Isend(&input, 10, MPI_INT, (rank + 1) % total, 1, MPI_COMM_WORLD, &request[0]);
		MPI_Wait(&request[0], &status[0]);
	}
	std::cout << "rank " << rank << " recived: " ;
	for (int i = 0; i < 10; ++i) {
		std::cout << input[i] <<"\t";
	}
	std::cout << std::endl;

}

int main(int argc, char** argv) {

	MPI_Init(NULL, NULL);

	MPI_Round();

	MPI_Shift();

	MPI_Finalize();

	return 0;
}
