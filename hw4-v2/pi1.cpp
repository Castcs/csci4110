// C++ code to implement computation of pi
#include <mpi.h>
#include <chrono>
#include <bits/stdc++.h>
#include <stdio.h>
#include <math.h>
using namespace std;
 
// Function to calculate PI
long double calcPI_segment(long double n, long double sign, long double iterations)
{
	long double PI_segment = 0.0;
    // Add for (iterations)
    for (unsigned long int i = 0; i <= iterations; i++) {
        PI_segment += (sign * (4 / ((n) * (n + 1) * (n + 2))));
        // Add and sub
        // alt sequences
        sign = sign * (-1);
        // Increment by 2 according to Nilkantha’s formula
        n += 2;
    }
 
    // Return the value of Pi
    return PI_segment;
}
 
// main
int main(int argc, char** argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Initialise variables, require/accept passed-in value 
    auto start = std::chrono::steady_clock::now();  // set timer
    long double PI = 3, n = 2, sign = 1;
    const long double PI25DT = 3.141592653589793238462643383; // set test value for error
    long double cPI = 0.0;
    
    if (argc==1)
    {
      printf("You must pass a single numeric value\n");
      printf("the value should be 100M or higher\n");
      return -1;
    }
    long double iterations, local_iterations, local_n, local_PI_segment, global_PI;
    iterations = std::stod(argv[1]); // set to passed-in numeric value

    // Split iterations among processes
    local_iterations = iterations / size;
    local_n = n + rank * local_iterations * 2; // each rank starts at a different segment
    sign = (rank % 2 == 0) ? 1 : -1; // adjust sign according to rank

    // Start timer
    //auto start = std::chrono::steady_clock::now();

    // Calculate segment of PI
    local_PI_segment = calcPI_segment(local_n, sign, local_iterations);

    // Collect results at root
    MPI_Reduce(&local_PI_segment, &global_PI, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Root process adds the base PI value and prints the result
    if (rank == 0) {
        global_PI += PI;
        printf("PI is approx %.50Lf, Error is %.50Lf\n", global_PI, fabsl(global_PI - PI25DT));

        // End timer
        auto end = std::chrono::steady_clock::now();
        auto diff = end - start;
        std::cout << std::chrono::duration<double, std::milli>(diff).count() << " Runtime ms" << std::endl;
    }

    MPI_Finalize();
    return 0;
}

