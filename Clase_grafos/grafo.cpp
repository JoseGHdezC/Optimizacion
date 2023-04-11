/*
 *  GRAFO.CPP - Plantilla para la implementación de la clase GRAFOS
 *
 *
 *  Autores : Antonio Sedeno Noda, Sergio Alonso
 *  Alumno  : José Gregorio Hernández Cruz
 *  Cursos  : 2012-2023
 */

#include <iostream>

#include "grafo.h"

void GRAFO::destroy() {
  for (unsigned i = 0; i < n; i++) {
	  LS[i].clear();
	  //A[i].clear();
	  if (dirigido == 1) {
      LP[i].clear();
    }
  }
	LS.clear();
	LP.clear();
	//A.clear();
}

void GRAFO::build(char nombrefichero[], int &errorapertura) {
  ElementoLista dummy;
  ElementoLista pred_dummy;
	ifstream textfile;
	textfile.open(nombrefichero);
	if (textfile.is_open()) {
		unsigned i, j, k;
		// leemos por conversion implicita el numero de nodos, arcos y el atributo dirigido
		textfile >> (unsigned &) n >> (unsigned &) m >> (unsigned &) dirigido;
		// los nodos internamente se numeran desde 0 a n-1
		// creamos las n listas de sucesores
		LS.resize(n);
    LP.resize(n);
        // leemos los m arcos
		for (k = 0; k < m; ++k) {
      //damos los valores a dummy.j y dummy.c
			textfile >> (unsigned &) i  >> (unsigned &) j >> (int &) dummy.c;
      dummy.j = j - 1;
			//situamos en la posicion del nodo i a dummy mediante push_back
      LS[i - 1].emplace_back(dummy);
			//Segundo push_back si es no dirigido. O no.
      pred_dummy = dummy;
      dummy.j = i - 1;
      if (dirigido == 0) {
        if (pred_dummy.j != dummy.j) { 
        LS[j - 1].emplace_back(dummy); 
        } 
      } 
      //Construcción de LP, si es dirigido
      if (dirigido == 1) {
        LP[j - 1].emplace_back(dummy); 
      }
			//pendiente del valor a devolver en errorapertura
      errorapertura = 0;
			//... 
    }
  } else {
    errorapertura = 1;
  } 
  textfile.close();
}

GRAFO::~GRAFO()
{
  destroy();
}

GRAFO::GRAFO(char nombrefichero[], int &errorapertura) {
	build (nombrefichero, errorapertura);
}

void GRAFO::actualizar(char nombrefichero[], int &errorapertura) {
    //Limpiamos la memoria dinamica asumida en la carga previa, como el destructor
    destroy();
    //Leemos del fichero y actualizamos G con nuevas LS y, en su caso, LP
    build(nombrefichero, errorapertura);
}

unsigned GRAFO::Es_dirigido() {
  return dirigido;
}

void GRAFO::Info_Grafo() {
  if (dirigido == 0) {
    std::cout << "Grafo no dirigido | Orden: " << n << " | Número de nodos: " << n << " | Número de aristas: " << m << std::endl;
  } 
  if (dirigido == 1) { 
    std::cout << "Grafo dirigido | Orden: " << n << " | Número de nodos: " << n << " | Número de arcos: " << m << std::endl;
  } 
}

void Mostrar_Lista(vector<LA_nodo> L) {
  for (int i = 0; i < L.size(); ++i) {
    std::cout << "[" << i + 1 << "]: "; 
    if (L[i].size() == 0) {
      std::cout << "null" << std::endl;
    } else { 
      for (int k = 0; k < L[i].size(); ++k) {
        std::cout << L[i][k].j + 1 << " /-> (" << L[i][k].c << ") ";  
      } 
      std::cout << std::endl;
    } 
  } 
}

void GRAFO::Mostrar_Listas (int l) {
  switch (l) {
    case 0:
    Mostrar_Lista(LS);
    break;
    case 1:
    Mostrar_Lista(LS);
    break;
    case -1:
    Mostrar_Lista(LP);
    break;
    default:
    std::cout << "Error: opción incorrecta" <<std::endl;
  } 
}

void GRAFO::Mostrar_Matriz() { //Muestra la matriz de adyacencia, tanto los nodos adyacentes como sus costes

}

//Recorrido en profundidad recursivo con recorridos enum y postnum
void GRAFO::dfs_num(unsigned i, vector<LA_nodo>  L, vector<bool> &visitado, vector<unsigned> &prenum, unsigned &prenum_ind, 
                    vector<unsigned> &postnum, unsigned &postnum_ind) {   
  visitado[i] = true;
  prenum[prenum_ind++] = i;//asignamos el orden de visita prenum que corresponde el nodo i
  for (unsigned j = 0; j < L[i].size(); j++) { 
    if (!visitado[L[i][j].j]) {
      dfs_num(L[i][j].j, L, visitado, prenum, prenum_ind, postnum, postnum_ind);
    }
  } 
  postnum[postnum_ind++] = i;//asignamos el orden de visita posnum que corresponde al nodo i
}

void GRAFO::RecorridoProfundidad() {
  unsigned number;
  std::cout << "Se va a realizar un recorrido en profundidad" << std::endl;
  std::cout << "Elige un nodo de partida [1 - " << n << "]: ";
  unsigned nodo;
  do { 
  std::cin >> number;
  } while (number < 1 && number > n); 
  nodo = number - 1;

  std::vector<bool> visitado;
  visitado.resize(n, false);

  std::vector<unsigned> prenum;
  prenum.resize(n);
  unsigned prenum_ind = 0;

  std::vector<unsigned> postnum;
  postnum.resize(n);
  unsigned postnum_ind = 0;

  dfs_num(nodo, LS, visitado, prenum, prenum_ind, postnum, postnum_ind);
  //Impresión del recorrido
  std::cout << "Orden del recorrido en preorden" << std::endl;
  for (int i = 0; i < n; ++i) {
    std::cout << "[" << prenum[i] + 1 << "]";
    if (i != n - 1) {
      std::cout << " -> ";
    }  
  } 
  std::cout << std::endl;

  std::cout << "Orden del recorrido en postorden" << std::endl;
  for (int i = 0; i < n; ++i) {
    std::cout << "[" << postnum[i] + 1 << "]";
    if (i != n - 1) {
      std::cout << " -> ";
    }  
  } 
  std::cout << std::endl;
}

//Recorrido en amplitud con la construcción de pred y d: usamos la cola
void GRAFO::bfs_num(unsigned i, //nodo desde el que realizamos el recorrido en amplitud
				            vector<LA_nodo>  L, //lista que recorremos, LS o LP; por defecto LS
				            vector<int> &pred, //vector de predecesores en el recorrido
				            vector<int> &d) //vector de distancias a nodo i+1
{
  vector<bool> visitado; //creamos e iniciamos el vector visitado
  visitado.resize(n, false);
  visitado[i] = true;

  pred.resize(n, -1); //creamos e inicializamos pred y d
  d.resize(n, -1);
  pred[i] = i;
  d[i] = 0;

  queue<unsigned> cola; //creamos e inicializamos la cola
  cola.push(i);//iniciamos el recorrido desde el nodo i+1

  while (!cola.empty()) {   //al menos entra una vez al visitar el nodo i+1 y contin�a hasta que la cola se vac�e
    unsigned k = cola.front(); //cogemos el nodo k+1 de la cola
    cola.pop(); //lo sacamos de la cola
    //Hacemos el recorrido sobre L desde el nodo k+1
    for (unsigned j = 0; j < L[k].size(); j++) {
    //Recorremos todos los nodos u adyacentes al nodo k+1
    //Si el nodo u no está visitado
      if (!visitado[L[k][j].j]) { 
        //Lo visitamos
        visitado[L[k][j].j] = true; 
        //Lo metemos en la cola
        cola.push(L[k][j].j);
        //le asignamos el predecesor
        pred[L[k][j].j] = k; 
        //le calculamos su etiqueta distancia
        d[L[k][j].j] = d[k] + 1;  
      } 
    }
        //Hemos terminado pues la cola está vacía
  }
}

void GRAFO::RecorridoAmplitud() {//Construye un recorrido en amplitud desde un nodo inicial
  unsigned number;
  std::cout << "Se va a realizar un recorrido en amplitud" << std::endl;
  std::cout << "Elige un nodo de partida [1 - " << n << "]: ";
  unsigned nodo;
  do { 
  std::cin >> number;
  } while (number < 1 && number > n); 
  nodo = number - 1;
  std::cout << "Nodo inicial: " << number << std::endl << std::endl;

  std::vector<int> pred;

  std::vector<int> d;

  bfs_num(nodo, LS, pred, d);
  //Impresión por pantalla
  std::cout << "Nodos según distancia al nodo inicial en número de aristas" << std::endl;
  int max_d = d[0]; 
  for (int i = 0; i < d.size(); ++i) {
    if (max_d < d[i]) {
      max_d = d[i]; 
    } 
  } 
  for (int i = 0; i <= max_d; ++i) {
    std::cout << "Distancia " << i << ": ";
    if (i == 0) {
      std::cout << number;
    } else {
      for (int j = 0; j < d.size(); ++j) {
        if (d[j] == i && d[j] != 0) {
          std::cout << j + 1 << " ";
        } 
      }
    }   
    std::cout << std::endl;
  } 
  std::cout << std::endl;
  std::cout << "Ramas de conexión en el recorrido" << std::endl;
  for (int i = 0; i < pred.size(); ++i) {
    if (nodo != i && pred[i] != -1) { 
      std::cout << pred[i] + 1 << " - " << i + 1 << std::endl; 
    } 
  } 
  std::cout << std::endl;
  //Si la distancia es -1, no se ha podido visitar el nodo durante el recorrido
  std::cout << "Vector distancia" << std::endl;
  for (int i = 0; i < d.size(); ++i) {
    std::cout << "[" << i + 1 << "] " << d[i] << " / ";
  }
  std::cout << std::endl << std::endl;
  //Si el predecesor es 0, no se ha podido visitar el nodo durante el recorrido
  std::cout << "Vector predecesores" << std::endl;
  for (int i = 0; i < pred.size(); ++i) {
    std::cout << "[" << i + 1 << "] " << pred[i] + 1 << " / ";
  }
  std::cout << std::endl;
}
