//Tomas Moro Lias 3A MAR-1 solucion al ejercicio 4


//Esta versión ligeramente modificada sacará los resultados de datos de entrada 100 a 5000 a un archivo .data para que GNU los haga grafico

#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <set>
using namespace std;

using vi = vector<int>; //Para simplificar el código usaré estos nombres
using ii = pair<int, int>;
using vii = vector<ii>;
using vvi = vector<vi>;
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
void OrdenacionTopologica(Grafo g);

/**
Dado un grafo devuelve otro con los aristas invertidos. Necesario para Kosaraju.
*/
Grafo revertirGrafo(Grafo &g);

/**
Un simple recorrido dfs para sacar por pantalla (y a un archivo) los resultados acorde al algoritmo Kosaraju.
*/
void dfsImprimir(int v, Grafo &g);

/**
Dado un tamaño devuelve un grafo listo para el uso de ese tamaño maximo.
*/
Grafo iniciarGrafo(const int &maxLocal);

/**
Implementacion del algoritmo kosaraju.
*/
void kosarajuCompConex(Grafo g);

int main(int argc, char **argv) {
	int n, maxLocal, m, a, b;
	ofstream file1;
	file1.open("ResultadosGNU.data");
	int c = 100;
	while (c <= 50000)
	{
		ifstream file;
		string s = "CasoGNU";
		s += to_string(c);
		s += ".txt";
		file.open(s);
		file >> n;
		while (n--)
		{
			int aristas = 0, nodos = 0;
			file >> maxLocal;
			//cout << maxLocal << endl;
			Grafo g = iniciarGrafo(maxLocal);
			file >> m;
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
				//cout << "Grafo ciclico: " << endl;
				kosarajuCompConex(g);
			}
			else {
				OrdenacionTopologica(g);
			}
			auto end = std::chrono::system_clock::now();
			chrono::duration<float, std::milli> duration = end - start;
			file1 << c << " " << duration.count() << endl;
		}
		file.close();
		c += 100;
	}
	file1.close();
	return 0;
}

//Una funcion recursiva para sacar vertices a partir de v
void dfsImprimir(int v, Grafo &g)
{
	g.cols[v] = NEGRO;
	//cout << v << " ";
	// Recursion para adyacentes
	for (auto a : g.listaAdyacencia[v]) {
		if (g.cols[a] == BLANCO) {
			dfsImprimir(a, g);
		}
	}
}


void kosarajuCompConex(Grafo g) {
	Grafo revertido = revertirGrafo(g);
	int compconex = 0;
	while (!revertido.stackOrden.empty())
	{
		compconex++;
		if (revertido.cols[revertido.stackOrden.top()] == BLANCO)
		{
			//cout << "Componente fuertemente conexa " << compconex << ": ";
			dfsImprimir(revertido.stackOrden.top(), revertido);
			//cout << endl;
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

Grafo iniciarGrafo(const int & maxLocal) {
	Grafo g = Grafo();
	g.listaAdyacencia = vvi(maxLocal + 1);
	//g.tiempos = vi(maxLocal+1);
	g.nodos = set<int>();
	g.cols = vector<colores>(maxLocal + 1);
	g.stackOrden = stack<int>();
	return g;
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

void OrdenacionTopologica(Grafo g) {

	while (!g.stackOrden.empty())
	{
		//cout << g.stackOrden.top() << endl;
		g.stackOrden.top();
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
