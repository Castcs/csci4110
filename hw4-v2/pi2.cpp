// C++ code to implement computation of pi
#include <bits/stdc++.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>
using namespace std;
 
// Function to calculate PI
long double calcPI(long double n, long double sign, long double iterations)
{
    long double PI = 0.0;
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
	long double local_PI = 0.0;
	long double *global_PI_ptr = nullptr;

    // Initialise variables, require/accept passed-in value
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
    	MPI_Alloc_mem(sizeof(long double), MPI_INFO_NULL, &global_PI_ptr);
	*global_PI_ptr = 0.0;
    	MPI_Win_create(global_PI_ptr, sizeof(long double), 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win);
    } else {
    	MPI_Win_create(NULL, 0, 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win);
    }
    auto start = std::chrono::steady_clock::now();
    MPI_Win_fence(0, win); // Opening of the first RMA epoch
    local_PI = calcPI(2.0 + (world_rank * 2), (world_rank % 2 == 0 ? 1 : -1), iterations_per_process);
    MPI_Put(&local_PI, 1, MPI_LONG_DOUBLE, 0, 0, 1, MPI_LONG_DOUBLE, win);
    MPI_Win_fence(0, win);

    if (world_rank == 0) {
        *global_PI_ptr += 3.0;
        printf("PI is approx %.50Lf, Error is %.50Lf\n", *global_PI_ptr, fabsl(*global_PI_ptr - PI25DT));
        auto end = std::chrono::steady_clock::now();
        std::cout << std::chrono::duration<double, std::milli>(end - start).count() << " Runtime ms" << std::endl;
    }

	if (world_rank == 0) {
        	MPI_Free_mem(global_PI_ptr);
    	}
	MPI_Win_free(&win);
	MPI_Finalize();
    return 0;
}

