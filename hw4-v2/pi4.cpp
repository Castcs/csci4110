// C++ code to implement computation of pi
#include <bits/stdc++.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
using namespace std;
long double calcPI_OpenMP(unsigned long iterations, int num_threads) {
    long double pi = 3.0;
    long double n = 2.0, sign = 1.0;
    #pragma omp parallel for reduction(+:pi) private(n, sign)
    for (unsigned long int i = 0; i < iterations; ++i) {
    	// Calculate the sign and the starting value of 'n' for this iteration and this thread
    	n = 2.0 + i * 2.0;  // You might need a better formula here to distribute 'n' across threads
    	sign = pow(-1.0, i);
    	pi += sign * (4.0 / (n * (n + 1.0) * (n + 2.0)));
    }
    return pi;
}
 
// main
int main(int argc, char** argv)
{
    long double cPI = 0.0;
    auto start = std::chrono::steady_clock::now();  // set timer
    const long double PI25DT = 3.141592653589793238462643383; // set test value for error
    
    if (argc < 3)
    {
      printf("args should be <iterations> <threads>\n");
      return -1;
    }
    int num_threads = std::stod(argv[2]);
    long double iterations = std::stod(argv[1]); // set to passed-in numeric value
    omp_set_num_threads(num_threads);
    cPI = calcPI_OpenMP(iterations, num_threads);
    printf("PI is approx %.50Lf, Error is %.50Lf\n", cPI, fabsl(cPI - PI25DT));
    auto end = std::chrono::steady_clock::now(); // end timer
    auto diff = end - start; // compute time
    std::cout << std::chrono::duration<double, std::milli>(diff).count() << " Runtime ms" << std::endl;
    return 0;
}

