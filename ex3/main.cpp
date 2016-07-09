/*
To compile:

g++ main.cpp -fopenmp -std=c++11 -o pi
tau_cxx.sh main.cpp -fopenmp -std=c++11 -o pi_tau
*/

#include <random>
#include <cstdio>
#include <omp.h>

#ifndef SIZE
	#define SIZE 10000000
#endif



int main(int argc, char const *argv[])
{
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

	double pi = 4.0*M/N;
	std::printf("PI is %f\n", pi);

	return 0;
}