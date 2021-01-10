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
	while (true) {
		cout << "Nombre del archivo (0 para terminar): ";
		cin >> s;
		if (s == "0") break;
		s += ".txt";
		ofstream file;
		file.open(s);
		cout << "Numero de casos: ";
		int n;
		long long e = 1e5;
		cin >> n;
		file << n << endl;
		//while (rand() % 100 > 15) {
		//	e *= 10; //numero de aristas
		//}
		for (int i = 0; i < n; i++)
		{
			int max = rand() % (int)(10e5);
			file << max << endl << e << endl;
			for (int i = 0; i < e; i++)
			{
				file << rand() % (int)(max) << ' ' << rand() % (int)(max) << endl;
			}
		}
		file.close();
	}
	return 0;
}