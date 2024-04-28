// C++ code to implement computation of pi
#include <bits/stdc++.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>
using namespace std;
 
// Function to calculate PI
long double calcPI(long double PI, long double n, long double sign, long double iterations)
{
    // Add for (iterations) 
    for (unsigned long int i = 0; i <= iterations; i++) {
        PI = PI + (sign * (4 / ((n) * (n + 1) * (n + 2))));
        // Add and sub
        // alt sequences
        sign = sign * (-1);
        // Increment by 2 according to Nilkantha’s formula
        n += 2;
    }
 
    // Return the value of Pi
    return PI;
}
 
// main
int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	int world_size, world_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	long double local_PI = 3.0;
	long double *global_PI_ptr;

    // Initialise variables, require/accept passed-in value 
    auto start = std::chrono::steady_clock::now();  // set timer
    long double PI = 3, n = 2, sign = 1;
    const long double PI25DT = 3.141592653589793238462643383; // set test value for error
    
    if (argc==1)
    {
      printf("You must pass a single numeric value\n");
      printf("the value should be 100M or higher\n");
      return -1;
    }

    long double iterations = std::stod(argv[1]); // set to passed-in numeric value    
    long double iterations_per_process = iterations / world_size;

    MPI_Win win;
    if (world_rank == 0) {
    	// Allocate memory for the global PI result at the master process
    	MPI_Alloc_mem(sizeof(long double), MPI_INFO_NULL, &global_PI_ptr);
    	*global_PI_ptr = 3.0; // Initialize global PI
    	MPI_Win_create(global_PI_ptr, sizeof(long double), sizeof(long double), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
    } else {
        // All other processes do not expose memory
    	MPI_Win_create(NULL, 0, sizeof(long double), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
    }

    MPI_Win_fence(0, win); // Opening of the first RMA epoch
    if (world_rank == 0) {
    	// Master process initializes its part of PI
    	*global_PI_ptr = calcPI(3.0, 2.0, 1, iterations_per_process);
    } else {
    	// Worker processes calculate their portion of PI
    	local_PI = calcPI(0.0, 2.0 + (world_rank * 2), (world_rank % 2 == 0 ? 1 : -1), iterations_per_process);
    	MPI_Put(&local_PI, 1, MPI_LONG_DOUBLE, 0, 0, 1, MPI_LONG_DOUBLE, win);
    }

    if (world_rank == 0) {
    	MPI_Win_fence(0, win); // Opening of the second RMA epoch
    	for (int i = 1; i < world_size; i++) {
        	// Use MPI_Get to retrieve the partial PI values from the workers
        	long double temp;
        	MPI_Get(&temp, 1, MPI_LONG_DOUBLE, i, 0, 1, MPI_LONG_DOUBLE, win);
        	*global_PI_ptr += temp;
    	}
   	// Synchronize after all MPI_Get calls
   	MPI_Win_fence(0, win);
    }

	MPI_Win_free(&win);
	if (world_rank == 0) {
        	MPI_Free_mem(global_PI_ptr);
    	}
	MPI_Finalize();

	// Only the master process should print the result
	if (world_rank == 0) {
		printf("PI is approx %.50Lf, Error is %.50Lf\n", *global_PI_ptr, fabsl(*global_PI_ptr - PI25DT));
		auto end = std::chrono::steady_clock::now(); // end timer
		auto diff = end - start; // compute time
		std::cout << std::chrono::duration<double, std::milli>(diff).count() << " Runtime ms" << std::endl;
	}
    return 0;
}

