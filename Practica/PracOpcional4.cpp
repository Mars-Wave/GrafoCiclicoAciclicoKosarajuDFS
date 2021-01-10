//Tomas Moro Lias 3A MAR-1 solucion al ejercicio 4

#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <set>
using namespace std;

using vi = vector<int>; //Para simplificar el código usaré estos nombres
using ii = pair<int, int>;
using vii = vector<ii>;
using vvi = vector<vi>;
using dqi = deque<int>;
const long MAX = 10e5;
typedef enum colores { BLANCO, GRIS, NEGRO };

typedef struct Grafo {
	vvi listaAdyacencia;
	//vi tiempos; no es necesario para este programa
	set<int> nodos = set<int>();
	vector<colores> cols;  //Usando el Cormen estos son Blanco Gris y Negro, similar a Estados: Bien, Tocado y Hundido
	stack<int> stackOrden = stack<int>();
};

/**En un recorrido dfs aparecen los siguientes aristas:
Tree edges : aristas que forman el arbol del DFS.
Back edges : aristas a vertices desde el vertice actual de la DFS hacia la raiz.
Forward edges : aristas a vertices alcanzables desde algun hijo ya explorado.
Cross edges(solo en grafos dirigidos) : aristas a otros vertices ya explorados con
anterioridad.

Utilizando una dfs se puede encontrar un ciclo si existe una back edge.

Esto es lo que hace esta función. Se ayuda de VisitarNodo.*/
bool dfsVisitas(Grafo &g);

/**
Función auxiliar a la DFS que representa un recorrido DFS, al volver sobre si mismo
a un nodo que ya ha visitado devuelve verdadero y para.

Tambien escribe en el stack del tipo de dato utilizado 'Grafo' los nodos por orden de finalizacion
de tal forma que se corresponda con el orden topologico en grafos aciclicos
o sino con el orden a seguir del algoritmo de Kosaraju para el grafo invertido.
*/
bool visitarNodo(int a, /*int &tiempo,*/ Grafo &g, vector<bool> previos = vector<bool>(MAX));

/**
Esta funcion utiliza el stack en el que se guarda el orden topologico y lo imprime por pantalla. Funciona para grafos
aciclicos. Para grafos ciclicos devuelve un orden pero no es topologico (entre otras cosas porque no puede serlo).
*/
void OrdenacionTopologica(Grafo g, ofstream &file1);

/**
Dado un grafo devuelve otro con los aristas invertidos. Necesario para Kosaraju.
*/
Grafo revertirGrafo(Grafo &g);

/**
Un simple recorrido dfs para sacar por pantalla (y a un archivo) los resultados acorde al algoritmo Kosaraju.
*/
void dfsImprimir(int v, Grafo &g, ofstream &file1);

/**
Dado un tamaño devuelve un grafo listo para el uso de ese tamaño maximo.
*/
Grafo iniciarGrafo(const int &maxLocal);

/**
Implementacion del algoritmo kosaraju.
*/
void kosarajuCompConex(Grafo g, ofstream &file1);

int main(int argc, char **argv) {
	int n, maxLocal, m, a, b, argms = argc;
	ofstream file1;
	file1.open("ResultadosDeEjecucion.txt");
	while (argms - 1)
	{
		argms--;
		file1 << "Numero de casos (de " << MAX << " nodos yendo de 0 a " << MAX << ") a procesar en el archivo " << argv[argms] << ": ";
		cout << "Numero de casos (de " << MAX << " nodos yendo de 0 a " << MAX << ") a procesar en el archivo " << argv[argms] << ": ";
		ifstream file;
		file.open(argv[argms]);
		file >> n;
		file1 << n << endl;
		cout << n << endl;
		while (n--)
		{
			int aristas = 0, nodos = 0;
			file1 << "Valor del arista mas grande: ";
			cout << "Valor del arista mas grande: ";
			file >> maxLocal;
			file1 << maxLocal << endl;
			cout << maxLocal << endl;
			Grafo g = iniciarGrafo(maxLocal);
			file1 << "Numero de aristas entre nodos a introducir para el caso " << n + 1 << ": ";
			cout << "Numero de aristas entre nodos a introducir para el caso " << n + 1 << ": ";
			file >> m;
			file1 << m << endl;
			cout << m << endl;
			file1 << endl << "Recibiendo nodos entre aristas... " << endl;
			cout << endl << "Recibiendo nodos entre aristas... " << endl;
			for (int i = 0; i < m; i++)
			{
				file >> a >> b;
				g.listaAdyacencia[a].push_back(b);
				g.nodos.insert(a);
				g.nodos.insert(b);
				aristas++;
			}
			auto start = std::chrono::system_clock::now();
			bool flag = dfsVisitas(g);
			nodos = g.nodos.size();
			if (flag)
			{
				file1 << endl << "Grafo ciclico, se procede a dar sus componentes fuertemente conexas como conjuntos de vertices:" << endl;
				cout << endl << "Grafo ciclico, se procede a dar sus componentes fuertemente conexas como conjuntos de vertices:" << endl;
				kosarajuCompConex(g, file1);
				file1 << endl;
				cout << endl;
			}
			else {
				file1 << endl << "Grafo aciclico, se procede a dar el ordenamiento topologico del grafo:" << endl;
				cout << endl << "Grafo aciclico, se procede a dar el ordenamiento topologico del grafo:" << endl;
				OrdenacionTopologica(g, file1);
				file1 << endl;
				cout << endl;
			}
			auto end = std::chrono::system_clock::now();
			chrono::duration<float, std::milli> duration = end - start;
			cout << "Ejecucion del algoritmo en: " << duration.count() << " ms para (Nodos+Aristas): " << nodos + aristas << " " << endl;
			file1 << "Ejecucion del algoritmo en: " << duration.count() << " ms para (Nodos+Aristas): " << nodos + aristas << " " << endl;
		}
		file.close();
	}
	file1.close();
	return 0;
}

//Una funcion recursiva para sacar vertices a partir de v
void dfsImprimir(int v, Grafo &g, ofstream &file1)
{
	g.cols[v] = NEGRO;
	file1 << v << " ";
	cout << v << " ";
	// Recursion para adyacentes
	for (auto a : g.listaAdyacencia[v]) {
		if (g.cols[a] == BLANCO) {
			dfsImprimir(a, g, file1);
		}
	}
}


void kosarajuCompConex(Grafo g, ofstream &file1) {
	Grafo revertido = revertirGrafo(g);
	int compconex = 0;
	while (!revertido.stackOrden.empty())
	{
		if (revertido.cols[revertido.stackOrden.top()] == BLANCO)
		{
			compconex++;
			file1 << "Componente fuertemente conexa " << compconex << ": ";
			cout << "Componente fuertemente conexa " << compconex << ": ";
			dfsImprimir(revertido.stackOrden.top(), revertido, file1);
			file1 << endl;
			cout << endl;
		}
		revertido.stackOrden.pop();
	}
}


//true si hay algun ciclo.
bool dfsVisitas(Grafo &g) {
	bool ciclo = false;
	set<int>::const_iterator it = g.nodos.begin();
	//int tiempo = 0;
	while (it != g.nodos.end())
	{
		int a = *it;
		//g.tiempos[a] = tiempo;
		g.cols[a] = BLANCO;
		it++;
	}
	it = g.nodos.begin();
	while (it != g.nodos.end())
	{
		int a = *it;
		if (g.cols[a] == BLANCO)
		{
			if (visitarNodo(a, /*tiempo,*/ g))
			{
				ciclo = true;
			}
		}
		it++;
	}
	return ciclo;
}

//previos empieza como vector de booleanos con todos sus valores en falso
//y acumula los nodos visitados en el recorrido, si vuelve sobre sus pasos
//y pasa por un nodo ya visitado en el recorrido actual entonces hay un ciclo
bool visitarNodo(int a, /*int &tiempo,*/ Grafo &g, vector<bool> previos) {
	bool ciclo = false;
	g.cols[a] = GRIS;
	//tiempo++;
	//g.tiempos[a] = tiempo;
	previos[a] = true;
	for (auto adj : g.listaAdyacencia[a])
	{
		if (previos[adj])
		{
			ciclo = true;
		}
		else if (g.cols[adj] == BLANCO)
		{
			if (visitarNodo(adj, /*tiempo,*/ g, previos)) {
				ciclo = true;
			}
		}
	}
	g.cols[a] = NEGRO;
	g.stackOrden.push(a);
	return ciclo;
}

Grafo iniciarGrafo(const int & maxLocal) {
	Grafo g = Grafo();
	g.listaAdyacencia = vvi(maxLocal + 1);
	//g.tiempos = vi(maxLocal+1);
	g.nodos = set<int>();
	g.cols = vector<colores>(maxLocal + 1);
	g.stackOrden = stack<int>();
	return g;
}

void OrdenacionTopologica(Grafo g, ofstream &file1) {

	while (!g.stackOrden.empty())
	{
		file1 << g.stackOrden.top() << endl;
		cout << g.stackOrden.top() << endl;
		g.stackOrden.pop();
	}
}


Grafo revertirGrafo(Grafo &g) {
	Grafo aux = g;
	aux.listaAdyacencia = vvi(MAX);
	set<int>::const_iterator it = g.nodos.begin();
	while (it != g.nodos.end())
	{
		int a = *it;
		aux.cols[a] = BLANCO;
		for (int adj : g.listaAdyacencia[a])
		{
			aux.listaAdyacencia[adj].push_back(a);
		}
		it++;
	}
	return aux;
}
