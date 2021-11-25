#include<iostream>
#include <string>
#include <math.h>

#define MAX_V 1123
#define INF 1123456

using namespace std;

struct Vertice{
    double x,y;

    Vertice(double x, double y):x(x),y(y){}
    Vertice():x(0),y(0){}
    string to_string(){ return std::to_string(x) +" "+ std::to_string(y); }
    void add_edge(struct Vertice b){
        
    }
};

struct Graph {
    int n;
    Vertice* vertices;
    double* edges;
    int* edge_visited;

    Graph(int n):n(n),vertices(NULL){
        vertices = (Vertice*)malloc(n*sizeof(Vertice));
        int n_vertices = n*(n-1)/2;
        edges = (double*)malloc(n_vertices*sizeof(double));
        edge_visited = (int*)malloc(n_vertices*sizeof(double));
    }
    double distance(Vertice a, Vertice b){ return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));}
    void add_edge(int a, int b){
        if(b<a){int aux=b;b=a;aux=b;} //troca os dois de posição caso seja 
        double dist = distance(vertices[a], vertices[b]);
        edges[b*(b-1)/2+a]=dist;
        edge_visited[b*(b-1)/2+a]=0;
    }
    void visit(int a, int b){
        if(b<a){int aux=b;b=a;aux=b;}
        edge_visited[b*(b-1)/2+a]=1;
    }
    void unvisit(int a, int b){
        if(b<a){int aux=b;b=a;aux=b;}
        edge_visited[b*(b-1)/2+a]=0;
    }
    double edge(int a, int b){
        if(b<a){int aux=b;b=a;aux=b;}
        return edges[b*(b-1)/2+a];
    }
};


// ---------------- LEITURA DO ARQUIVO --------------------
Graph read_points(char *file_name){
    FILE *fp = fopen("input.txt", "r");
    
    if (fp == NULL)
	{
		fprintf(stderr, "Falha ao criar input.txt.\n");
        return Graph(0);
	}
    int n;
    fscanf(fp,"%d", &n);
    Graph G = Graph(n);
    for(int i=0; i<G.n; i++){
        double x, y;
        fscanf(fp,"%lf %lf", &x, &y);
        G.vertices[i]=Vertice(x,y);
        cout << G.vertices[i].to_string() << endl;
        for (int j=0; j<i; j++){
            G.add_edge(i,j);
        }
    }
    

    fclose(fp);

    return G;
}


// ------------------------------------------------------


int main(int argc, char *argv[]){
    if (argc != 2) {
		fprintf(stderr, "Uso: %s input.txt\n", argv[0]);
		return 1;
	}

    Graph G = read_points(argv[1]);
    if(G.n<2){
        return 1;
    }

    for(int i=0; i<G.n; i++)
        cout << G.vertices[i].to_string() << endl;
    
    clock_t t;
    t = clock();
    //Edge r = EdgeMaisProximo(points, n_points);  //Solução O(n log n)
    t = clock()-t;
    
    //cout << (float)t/CLOCKS_PER_SEC << " " << r.to_string() << endl;
    

    return 0;
}