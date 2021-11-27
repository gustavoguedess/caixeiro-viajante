#include<iostream>
#include <string>
#include <math.h>

#include <ostream>
#include <fstream>

#define MAX_V 1123
#define INF 1123456

using namespace std;


struct Edge{
    int v;
    double c;
    Edge(int v, int c):v(v),c(c){};
};

struct Vertice{
    double x,y;
    int visit;
    double c;
    int p;
    int adj_size;
    Edge* adj;

    Vertice(double x, double y):x(x),y(y), visit(0), c(INF),adj_size(0),adj(NULL){}
    string to_string(){ return std::to_string(x) +" "+ std::to_string(y); }
    void set_amount_adj(int n){adj = (Edge*)malloc(n*sizeof(Edge));}
    void add_adj(int i, int c){
        adj[adj_size++] = Edge(i,c);
    }
};

struct Graph {
    int n;
    Vertice* v;

    Graph(int n):n(n),v(NULL){
        v = (Vertice*)malloc(n*sizeof(Vertice));
    }
    double distance(Vertice a, Vertice b){ return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));}
    void add_edge(int a, int b){
        double dist = distance(v[a], v[b]);
        v[a].add_adj(a,dist);
        v[b].add_adj(b,dist);
    }
};

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
    Graph G = Graph(n);

    for(int i=0; i<G.n; i++){
        double x, y;
        fscanf(fp,"%lf %lf", &x, &y);
        G.v[i]=Vertice(x,y);
        G.v[i].set_amount_adj(n);

        for(int j=0; j<i; j++){
            int c = G.distance(G.v[i], G.v[j]);
            G.v[i].add_adj(j,c);
            G.v[j].add_adj(i,c);
        }
    }

    fclose(fp);

    return G;
}

void save_graph(Graph G, string name){
    std::ofstream example(name);
    std::ostream& os = example;

    /*
    FILE *fp = fopen("tree_complete.txt", "w");
    if (fp == NULL)
	{
		fprintf(stderr, "Falha ao criar tree_complete.txt.\n");
		return;
	}*/

    for(int i=0; i<G.n; i++){
        for(int j=0; j<G.v[i].adj_size; j++){
            os << G.v[i].to_string() << endl;
            os << G.v[G.v[i].adj[j].v].to_string() << endl;
        }
    }
}

void print_graph(Graph G){
    for(int i=0; i<G.n; i++){
        cout << G.v[i].to_string();
        cout << "\t[";
        for(int j=0; j<G.v[i].adj_size; j++){
            cout << j << ",";
        }
        cout << "]" << endl;
    }
}
// ------------------------------------------------------

int extract_min(Graph G){
    int u=-1;
    for(int i=0; i<G.n; i++){
        if(u==-1 && G.v[i].visit==0) u=i;
        if(G.v[i].visit==0 && G.v[i].c<G.v[u].c)
            u=i;
    }
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
        cout << u << " " << G.v[u].c << endl;
        Edge* adj = G.v[u].adj;
        int v;
        double c;
        for(int j=0 ; j<G.v[u].adj_size; j++){
            v = adj[j].v;
            c = adj[j].c;
            if(G.v[u].c+c < G.v[v].c){
                G.v[v].c = G.v[u].c+c; 
                G.v[v].p = u;
            }
        }
        G.v[u].visit=1;
    }
    Graph P(G.n);
    for(int i=0; i<G.n; i++){P.v[i]=Vertice(G.v[i].x,G.v[i].y); P.v[i].set_amount_adj(G.n);}
    for(int i=0; i<G.n; i++){
        cout << i << " pai:" << G.v[i].p << endl;
        if(i!=r) P.add_edge(i,G.v[i].p);
    }

    return P;
}
void approx_tsp_tour(Graph G){
    int r = 0;
    Graph P = mst_prim(G, r);
    print_graph(P);
    save_graph(P, "tree.txt");
}

int main(int argc, char *argv[]){
    if (argc != 2) {
		fprintf(stderr, "Uso: %s input.txt\n", argv[0]);
		return 1;
	}

    Graph G = read_points(argv[1]);
    if(G.n<2){
        return 1;
    }
    save_graph(G, "build/graph_complete.txt");

    clock_t t;
    t = clock();
    approx_tsp_tour(G);
    t = clock()-t;
    
    cout << (float)t/CLOCKS_PER_SEC << endl;
    

    return 0;
}