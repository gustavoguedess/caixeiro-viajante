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
    int adj_size;
    vector<int> adj;

    Vertice(int x, int y):x(x),y(y),adj_size(0){}
    string to_string(){ return std::to_string(x) +" "+ std::to_string(y); }
    void add_adj(int i){
        adj.push_back(i);
        adj_size++;
    }
};

struct Graph {
    int n;
    vector<Vertice> v;

    Graph():n(0){}
    void add_vertice(Vertice b){ v.push_back(Vertice(b.x, b.y)); n+=1;}
    void add_vertice(int x, int y){
        v.push_back(Vertice(x,y));
        n+=1;
    }
    double distance(Vertice a, Vertice b){ return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));}
    void add_edge(int a, int b){
        v[a].add_adj(b);
    }

    //*******************************************
    //TODO remover depois
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
    //********************************************
};
struct Heap{
    vector<int> v;
    vector<int> cost;
    int n;
    Heap():n(0){v.push_back(0);};
    
    void add_vertice(int i, int c){
        v.push_back(i);
        cost.push_back(c);
        n++;
    }
    int extract_min(){
        int min_heap = v[1];
        v[1] = v[n];
        n--;
        min_heapify(1);
        return min_heap;
    }
    void update(int i){
        int p = i/2;
        if(p>0 && cost[v[i]]<cost[v[p]]){
            change(i,p);
            update(p);
        }
    }
    void min_heapify(int i){
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
    void print(){
        for( int i=1; i<=n; i++)
            cout << v[i] << ":" << cost[v[i]] << " ";
        cout << endl;
    }
};

Graph T;

// ---------------- LEITURA DO ARQUIVO --------------------
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

void save_tree(string name){
    std::ofstream example(name);
    std::ostream& os = example;

    for(int i=0; i<T.n; i++){
        for(int j=0; j<T.v[i].adj_size; j++){
            int v=T.v[i].adj[j];
            if(i!=v){
                os << T.v[i].to_string() << endl;
                os << T.v[v].to_string() << endl;
            }
        }
    }
}

/*
void save_cycle(Graph G, string name){
    std::ofstream example(name);
    std::ostream& os = example;

    for(int i=0; i<=G.n; i++)
        os << G.v[i%G.n].to_string() << endl;
    
}

void print_graph(Graph G){
    cout << G.to_string() << endl;
}
*/
// ------------------------------------------------------

Graph mst_prim(Graph G, int r){
    Heap H;
    vector<int> visited(G.n);
    vector<int> parent(G.n);

    
    for(int i=0; i<G.n; i++){
        H.add_vertice(i,INF);
        T.add_vertice(G.v[i]);
        visited[i]=0;
    }
    H.cost[r]=0;
    parent[r]=r;
    H.update(r+1);

    while(H.n>0){
        int u = H.extract_min();
        T.add_edge(parent[u], u);
        visited[u]=1;
        
        for(int i=1; i<=H.n; i++){
            int v = H.v[i];
            float dist = G.distance(G.v[u],G.v[v]);
            if(dist < H.cost[v]){
                H.cost[v] = dist;
                parent[v] = u; 
                H.update(i);
            }
        }
        
    }

    return T;
}
/*
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
*/
void approx_tsp_tour(Graph G){
    int r = 0;
    //for(int i=0; i<G.n; i++) if(G.v[i].y<G.v[r].y) r=i;

    cout << "Calculando Prim..." << endl;
    T = mst_prim(G, r);
    cout << "Ok!" << endl;
    
  
    //cout << "Calculando Pré Ordem..." << endl;
    //H = pre_ordem(T, r);
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
    save_tree("build/tree.txt");
    //cout << "Salvando o ciclo..." << endl;
    //save_cycle(H, "build/cycle.txt");
    cout << "Ok!" << endl;
    
    return 0;
}