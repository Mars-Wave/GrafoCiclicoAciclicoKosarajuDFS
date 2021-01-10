#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
#include <stdio.h>
#include <time.h>
using namespace std;

int main() {
	srand(time(NULL));
	string s;
	int i = 100;
	while (i <= 50000) {
		s = "CasoGNU";
		s += to_string(i);
		s += ".txt";
		cout << s << endl;
		ofstream file;
		file.open(s);
		int n = 1;
		file << n << endl;
		for (int m = 0; m < n; m++)
		{
			int max = 2*i;
			file << max << endl << i << endl;
			for (int j = 0; j < i; j++)
			{
				int r1 = rand() % (int)(max);
				int r2 = rand() % (int)(max);
				while (r1 == r2)
				{
					r2 = rand() % (int)(max);
				}
				file << r1 << ' ' << r2 << endl;
			}
		}
		file.close();
		i += 100;
	}
	cout << "Terminado."<<endl;
	return 0;
}