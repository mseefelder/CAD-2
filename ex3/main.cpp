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
	#define SIZE 500
#endif
#ifndef TIMES
	#define TIMES 100000
#endif

double calculatePI () {
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
	return pi;
}

int main(int argc, char const *argv[])
{
	const long double PI = 3.141592653589793238L;
	long double accumPi = 0.0L;
	long double accumPiDelta = 0.0L;
	long double accumVariance = 0.0L;
	
	struct timeval start, end;
	gettimeofday(&start, NULL);

	long double* pi = new long double[TIMES];
	for (int i = 0; i < TIMES; ++i)
	{
		pi[i] = calculatePI();
	}

	gettimeofday(&end, NULL);
	float seconds = ((end.tv_sec  - start.tv_sec) * 1000000u + 
		end.tv_usec - start.tv_usec) / 1.e6;
	printf("Found %i PI in %f seconds.\n", TIMES, seconds);

	for (int i = 0; i < TIMES; ++i)
	{
		accumPi += pi[i];
		accumPiDelta += std::abs(PI-pi[i]);
	}

	printf("Mean PI is %10Le\n", accumPi/(long double)TIMES);

	accumPiDelta = accumPiDelta/(long double)TIMES;

	for (int i = 0; i < TIMES; ++i)
	{
		pi[i] = accumPiDelta - std::abs(PI-pi[i]);
		pi[i] = pi[i]*pi[i];
		accumVariance += pi[i];
	}

	long double stdDev = std::sqrt(accumVariance/(long double)TIMES);

	printf("Difference from PI mean is %10Le and Standard Deviation is %10Le\n", accumPiDelta, stdDev);

	return 0;
}