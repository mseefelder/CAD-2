/*
COMPILE WITH:
g++ plotmaker.cpp -std=c++11 -o plotmaker

FOR THE WAVE APPLICATION, WITH DEFAULT PARAMENTERS, USE:
./plotmaker wave 200 10
*/

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>

int main(int argc, char const *argv[])
{
	if (argc < 4) 
	{ // Check the value of argc. If not enough parameters have been passed, inform user and exit.
		std::cout << "Usage is: \n"<<argv[0]<<
			" <rootname> <number of iterations> <plot step>\n"<<
			"Press any key to close... \n";
		std::cin.get();
		return 0;
	}

	int step_limit = std::stoi(argv[2]);
	int step_size = std::stoi(argv[3]);
	int t_step = 0;
	char fname[80];
	FILE* fout;
	for (t_step = 0; t_step < step_limit; t_step += step_size)
	{
		sprintf(fname,"%s_%03d.plot", argv[1], t_step);
		fout = fopen(fname,"w");
		fprintf(fout, "set terminal png\n");
		fprintf(fout, "set output \'%s_%03d.png\'\n", argv[1], t_step);
		fprintf(fout, "set xlabel \'x\'\n");
		fprintf(fout, "set xlabel \'y\'\n");
		fprintf(fout, "set pm3d map\n");
		fprintf(fout, "set palette gray\n");
		fprintf(fout, "set dgrid3d 100,100\n");
		fprintf(fout, "splot \'%s_%03d.dat\' u 1:2:3 t\"\"\n", argv[1], t_step);
		fclose(fout);	
	}
	return 0;
}