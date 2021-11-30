#include<iostream>
#include <string>
#include <math.h>

#include <ostream>
#include <fstream>
#include <vector>

#define MAX_V 1123
#define INF 1123456

using namespace std;

struct Vertice{
    int x,y;
    int visit;
    double c;
    int p;
    int adj_size;
    vector<int> adj;

    Vertice(int x, int y):x(x),y(y), visit(0), c(INF),adj_size(0){}
    string to_string(){ return std::to_string(x) +" "+ std::to_string(y); }
    void add_adj(int i){
        adj.push_back(i);
        adj_size++;
    }
};

struct Graph {
    int n;
    Vertice* v;

    Graph(int n):n(n),v(NULL){
        v = (Vertice*)malloc(n*sizeof(Vertice));
    }
    Graph(){}
    double distance(Vertice a, Vertice b){ return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));}
    void add_edge(int a, int b){
        double dist = distance(v[a], v[b]);
        v[a].add_adj(b);
        v[b].add_adj(a);
    }
    string print_adj(int a){
        string tostring = "[";
        for (int i=0; i<v[a].adj_size; i++)
            tostring+=std::to_string(v[a].adj[i])+",";
        tostring+="]";
        return tostring;
    }
    string to_string(){
        string tostring = "";
        for(int i=0; i<n; i++){
            tostring+= std::to_string(i) +": "+v[i].to_string()+" "+print_adj(i)+"\n";
        }
        return tostring;
    }
};


Graph T, H;

// ---------------- LEITURA DO ARQUIVO --------------------
Graph read_points(char *file_name){
    FILE *fp = fopen(file_name, "r");
    
    if (fp == NULL)
	{
		fprintf(stderr, "Falha ao ler a entrada.\n");
        return Graph(0);
	}
    int n;
    fscanf(fp,"%d", &n);
    Graph R(n);

    for(int i=0; i<R.n; i++){
        int x, y;
        fscanf(fp,"%d %d", &x, &y);
        R.v[i]=Vertice(x,y);
    }

    fclose(fp);

    return R;
}

void save_graph(Graph G, string name){
    std::ofstream example(name);
    std::ostream& os = example;

    for(int i=0; i<G.n; i++){
        for(int j=0; j<G.v[i].adj_size; j++){
            os << G.v[i].to_string() << endl;
            os << G.v[G.v[i].adj[j]].to_string() << endl;
        }
    }
}
void save_cycle(Graph G, string name){
    std::ofstream example(name);
    std::ostream& os = example;

    for(int i=0; i<=G.n; i++)
        os << G.v[i%G.n].to_string() << endl;
    
}

void print_graph(Graph G){
    cout << G.to_string() << endl;
}
// ------------------------------------------------------
clock_t total = 0;
int extract_min(Graph G){
    clock_t t;
    t = clock();

    int u=-1;
    for(int i=0; i<G.n; i++){
        if(u==-1 && G.v[i].visit==0) u=i;
        if(G.v[i].visit==0 && G.v[i].c<G.v[u].c)
            u=i;
    }
    t = clock()-t;
    total+=t;
    return u;
}
Graph mst_prim(Graph G, int r){
    for(int i=0; i<G.n; i++){
        G.v[i].c=INF;
        G.v[i].visit=0;
    }
    G.v[r].c=0;
    for(int i=0; i<G.n; i++){
        int u = extract_min(G);
        G.v[u].visit=1;
        
        double c;
        double dist;
        for(int v=0 ; v<G.n; v++){
            dist = G.distance(G.v[u],G.v[v]);
            if(dist < G.v[v].c && !G.v[v].visit){
                G.v[v].c = dist; 
                G.v[v].p = u;
            }
        }
    }

    
    clock_t t;
    t = clock();
    Graph P(G.n);
    for(int i=0; i<G.n; i++){P.v[i]=Vertice(G.v[i].x,G.v[i].y);}
    for(int i=0; i<G.n; i++){
        //cout << i << " pai:" << G.v[i].p << endl;
        if(i!=r) P.add_edge(i,G.v[i].p);
    }
    t = clock()-t;
    cout << (float)t/CLOCKS_PER_SEC << " segundos criando o grafo da arvore minima" << endl;
    return P;
}
int dfs(Graph T, int u, int ordem[], int pos){
    ordem[pos++]=u;
    T.v[u].visit=1;

    int v;
    for(int i=0; i<T.v[u].adj_size; i++){
        v = T.v[u].adj[i];
        if (!T.v[v].visit){
            pos = dfs(T, v, ordem, pos);
        }
    }
    return pos;
}

Graph pre_ordem(Graph T, int r){
    int* ordem = (int*)malloc(T.n*sizeof(int));
    int pos=0;

    for(int i=0; i<T.n; i++) T.v[i].visit=0;
    dfs(T, r, ordem, pos);

    Graph H(T.n);
    for(int i=0; i<T.n; i++){H.v[i]=Vertice(T.v[ordem[i]].x,T.v[ordem[i]].y);}
    return H;
}
void approx_tsp_tour(Graph G){
    int r = 0;
    //for(int i=0; i<G.n; i++) if(G.v[i].y<G.v[r].y) r=i;
    cout << "r: " << r << endl;

    cout << "Calculando Prim..." << endl;
    T = mst_prim(G, r);
    cout << "Ok!" << endl;
    
    cout << (float)total/CLOCKS_PER_SEC << " segundos extraindo o minimo" << endl;

    cout << "Calculando Pré Ordem..." << endl;
    H = pre_ordem(T, r);
}

int main(int argc, char *argv[]){
    if (argc != 2) {
		fprintf(stderr, "Uso: %s input.txt\n", argv[0]);
		return 1;
	}

    Graph R = read_points(argv[1]);
    if(R.n<2){
		fprintf(stderr, "Grafo pequeno\n");
        return 1;
    }
    clock_t t;
    t = clock();
    approx_tsp_tour(R);
    t = clock()-t;
    
    cout << (float)t/CLOCKS_PER_SEC << endl;

    
    cout << "Salvando..." << endl;
    cout << "Salvando a árvore mínima..." << endl;
    save_graph(T, "build/tree.txt");
    cout << "Salvando o ciclo..." << endl;
    save_cycle(H, "build/cycle.txt");
    cout << "Ok!" << endl;
    
    return 0;
}