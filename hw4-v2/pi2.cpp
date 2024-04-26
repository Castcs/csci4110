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
	MPI_Win win;
    	MPI_Win_create(&local_PI, sizeof(long double), sizeof(long double), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
	    MPI_Win_fence(0, win);
    long double iterations = std::stod(argv[1]); // set to passed-in numeric value    
    if (world_rank == 0) {
        // Master process
        long double global_PI = 0.0;
        for (int i = 1; i < world_size; i++) {
            long double temp;
            MPI_Get(&temp, 1, MPI_LONG_DOUBLE, i, 0, 1, MPI_LONG_DOUBLE, win);
            global_PI += temp;
        }
        global_PI /= world_size;
        // Output the result
    } else {
        // Worker processes
        local_PI = calcPI(3.0, 2.0, (world_rank % 2 == 0 ? 1 : -1), iterations);
        MPI_Put(&local_PI, 1, MPI_LONG_DOUBLE, 0, 0, 1, MPI_LONG_DOUBLE, win);
    }

    	// End the one-sided epoch.
    	MPI_Win_fence(0, win);

    	MPI_Win_free(&win);
    	MPI_Finalize();

    printf("PI is approx %.50Lf, Error is %.50Lf\n", global_PI, fabsl(global_PI - PI25DT));
    auto end = std::chrono::steady_clock::now(); // end timer
    auto diff = end - start; // compute time
    std::cout << std::chrono::duration<double, std::milli>(diff).count() << " Runtime ms" << std::endl;
    return 0;
}

