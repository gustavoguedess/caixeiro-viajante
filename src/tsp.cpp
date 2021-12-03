/**************************************************************
 *                                                            *
 *      Trabalho 2 de Projeto e Análise de Algoritmo          *
 *                      Caixeiro Viajante                     *
 *      Escrito por:                                          *
 *          Gustavo Inacio Pereira Guedes - 2088860           *
 *          Franziska Romani Furtado - 1248006                *
 *          Rafael Rosa da Silva - 2024551                    *
 *                                                            *
 * ************************************************************/

#include<iostream>
#include <string>
#include <math.h>

#include <ostream>
#include <fstream>
#include <vector>
#include <time.h>

#define MAX_V 1123
#define INF 1123456

using namespace std;

struct Vertice{
    int x,y; //Coordenadas deles
    int adj_size; //Quantidade de vértices adjacentes
    vector<int> adj; //Indice dos vértices adjacentes

    Vertice(int x, int y):x(x),y(y),adj_size(0){}
    void add_adj(int i){
        adj.push_back(i);
        adj_size++;
    }
};

struct Graph {
    int n; //Quantidade de vértices
    vector<int> order; //Ordem da dfs para o ciclo
    vector<Vertice> v; //Lista de Vértices

    Graph():n(0){}
    void add_vertice(Vertice b){ v.push_back(Vertice(b.x, b.y)); n+=1;}
    void add_vertice(int x, int y){
        v.push_back(Vertice(x,y));
        n+=1;
    }
    //Calculo da distancia
    double distance(Vertice a, Vertice b){ return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));}
    //Adiciona o vértice só em uma direção (na direção de visita da árvore)
    void add_edge(int a, int b){
        v[a].add_adj(b);
    }
};
struct Heap{
    vector<int> v; //Heap com os índices dos vértices 
    vector<int> cost; //Lista de custo dos vértices
    int n; //
    Heap():n(0){v.push_back(0);};

    void add_vertice(int i, int c){
        v.push_back(i);
        cost.push_back(c);
        n++;
    }
    int extract_min(){
        //Sempre retorna o vértice mais acima
        int min_heap = v[1];

        //coloca o último nó na primeira posição e faz ele cair para atualizar a Heap
        v[1] = v[n];
        n--;
        min_heapify(1);
        return min_heap;
    }
    int parent(int i){return i/2;}
    void change_cost(int i, int c){
        //diminui o custo do vértice da heap
        cost[v[i]]=c;
        //Faz ele subir enquanto seja menor que o vértice pai
        while (i>1 && cost[v[i]]<cost[v[parent(i)]]){
            change(i,parent(i));
            i=parent(i);
        }
    }
    void min_heapify(int i){
        //Flutua pra baixo enquanto tenha algum menor
        int e = i*2;
        int d = i*2+1;
        int m = i;
        if(e<=n && cost[v[e]]<cost[v[m]]) m=e;
        if(d<=n && cost[v[d]]<cost[v[m]]) m=d;

        if(m!=i){
            change(m,i);
            min_heapify(m);
        }
    }
    void change(int i, int j){
        int aux = v[i];
        v[i]=v[j];
        v[j]=aux;
    }
};

Graph T;

// ---------------- LEITURA E ESCRITA DO ARQUIVO --------------------
Graph read_points(char *file_name){
    FILE *fp = fopen(file_name, "r");

    if (fp == NULL)
	{
		fprintf(stderr, "Falha ao ler a entrada.\n");
        return Graph();
	}
    int n;
    fscanf(fp,"%d", &n);
    Graph R;

    for(int i=0; i<n; i++){
        int x, y;
        fscanf(fp,"%d %d", &x, &y);
        R.add_vertice(x,y);
    }

    fclose(fp);

    return R;
}

void save_tree(const char* filename, Graph T){
    FILE* fp = fopen(filename, "w");

    for(int i=0; i<T.n; i++){
        for(int j=0; j<T.v[i].adj_size; j++){
            int v = T.v[i].adj[j];
            if(i!=v){
                fprintf(fp, "%d %d\n", T.v[i].x, T.v[i].y);
                fprintf(fp, "%d %d\n", T.v[v].x, T.v[v].y);
            }
        }
    }

    fclose(fp);
}

void save_cycle(const char* filename, Graph T){
    FILE* fp = fopen(filename, "w");

    for(int i=0; i<=T.n; i++){
        int u = T.order[i%T.n];
        fprintf(fp, "%d %d\n", T.v[u].x, T.v[u].y);
    }
    fclose(fp);
}
// ------------------------------------------------------

void mst_prim(Graph* T, int r=0){
    Heap H;
    vector<int> visited(T->n);
    vector<int> parent(T->n);


    //Cria a Heap e marca todos como não visitados
    for(int i=0; i<T->n; i++){
        H.add_vertice(i,INF); //Instancia a Heap com valores infinitos
        visited[i]=0;
    }
    //Atualiza o custo do primério vértice
    parent[r]=r;
    H.change_cost(r+1, 0);

    while(H.n>0){
        //Extrai vértice, adiciona ele na árvore mínima e marca como visitado
        int u = H.extract_min();
        T->add_edge(parent[u], u);
        visited[u]=1;

        //Atualiza cada vizinho, caso tenha um custo menor
        for(int i=1; i<=H.n; i++){
            int v = H.v[i];
            float dist = T->distance(T->v[u],T->v[v]);
            if(dist < H.cost[v]){
                parent[v] = u;
                H.change_cost(i, dist); //Atualiza a Heap para o vértice subir caso seja menor
            }
        }

    }

}

void dfs(Graph* T, int u, vector<int>& visit){
    //Adiciona na fila
    T->order.push_back(u);
    visit[u]=1;

    int v;
    for(int i=0; i<T->v[u].adj_size; i++){
        v = T->v[u].adj[i];
        //Faz a visita de todos os adjacentes que não foram visitados
        if (!visit[v]){
            dfs(T, v, visit);
        }
    }
}

double cycle_cost(Graph* T){
    double cost=0;
    //Calcula o custo de todos, incluindo do último para o primeiro
    for(int i=1; i<=T->n; i++){
        int u = (i-1)%T->n;
        int v = i%T->n;
        cost+=T->distance(T->v[u],T->v[v]);
    }
    return cost;
}

double pre_order(Graph* T, int r=0){
    vector<int> visit;

    //Marca todos como visitado
    for(int i=0; i<T->n; i++) visit.push_back(0);
    
    //Faz a DFS para calcular a pré ordem
    dfs(T, r, visit);
    
    //Calcula o custo da pré ordem
    return cycle_cost(T);
}

int main(int argc, char *argv[]){
    clock_t t;
    t = clock();

    Graph T = read_points(argv[1]);

    //Árvore mínima, gera os vértices da MST no próprio T
    mst_prim(&T);

    //Faz a pré ordem e calcula o custo dela
    double cost = pre_order(&T);

    //Salva os dados
    save_tree("build/tree.txt", T);
    save_cycle("build/cycle.txt", T);


    t = clock()-t;
    double time = (double)t/CLOCKS_PER_SEC;
    printf("%lf %lf\n", time, cost);

    return 0;
}
