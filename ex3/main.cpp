/*
To compile:

g++ main.cpp -fopenmp -std=c++11 -o pi
tau_cxx.sh main.cpp -fopenmp -std=c++11 -o pi_tau
*/

#include <random>
#include <cstdio>
#include <sys/time.h>
#include <omp.h>

#ifndef SIZE
	#define SIZE 10000000
#endif



int main(int argc, char const *argv[])
{
	struct timeval start, end;
	gettimeofday(&start, NULL);

	double PI = 3.14159265359;
	double M, N;
	#pragma omp parallel reduction(+:M,N)
	{
		double x, y, length;
		int tn = omp_get_thread_num();
		M = 0.0; N = 0.0;
		std::random_device rd;
	    std::mt19937 gen(rd());
	    std::uniform_real_distribution<> dis(-0.5, 0.5);

	    for (int i = 0; i < SIZE; ++i)
	    {
	    	x = dis(gen);
	    	y = dis(gen);

	    	length = x*x + y*y;
	    	N++;
	    	M = (length <= 0.25) ? M+1.0 : M;
	    }

	}
	#pragma omp barrier

	double pi = 4.0*M/N;
	std::printf("PI is %f difference: %e\n", pi, std::abs(PI-pi));

	gettimeofday(&end, NULL);
	float seconds = ((end.tv_sec  - start.tv_sec) * 1000000u + 
		end.tv_usec - start.tv_usec) / 1.e6;
	printf("Found PI in %f seconds.\n", seconds);

	return 0;
}