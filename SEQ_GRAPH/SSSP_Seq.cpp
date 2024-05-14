#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

typedef struct Edge{
    int from;
    int to;
    float edgeweight;
} edge;

bool Compare(Edge e1, Edge e2){
    return e1.from < e2.from;
}

class Graph{

    bool updated = false;
    int Edges =0;
    int Vertics=0;
    int depth = 0;

    public:

    vector<edge> Nodes;
    vector<int> sssp;
    vector<int> isum;
    vector<bool> visited;
    Graph(int edges,int vertices):
    sssp(vertices,INT_MAX),visited(vertices,false),isum(vertices+1,0)
    {
        sssp[0] = 0;
        Edges = edges;
        Vertics = vertices;
    }

    void rowcount(){
        int value=0,count=0,last=0;
        for(int i=0;i<Nodes.size();i++){
            if(Nodes[i].from != last){
                while(value!=Nodes[i].from){
                    isum[value] = count; 
                    value++;
                }
                last = Nodes[i].from;
                isum[value] = count;
                value++;
            }
            count++;
        }
        isum[value] = count;
        for(int i=value;i<Vertics+1;i++){
            isum[i] = count;
        }
        isum[0] = 0;
    }

    void SSSP(){
        bool updated = false;
        while(1){ //relaxation of vertices for vertices-1 times
            updated = false;
            for(int i=0;i<Edges;i++){
                if(sssp[Nodes[i].from]+Nodes[i].edgeweight<sssp[Nodes[i].to]) {
                    sssp[Nodes[i].to] = sssp[Nodes[i].from]+Nodes[i].edgeweight;
                    updated = true;
                }
            }
            if(!updated) break;
        }
    }

    void correctness(){
        for(int i=0;i<Edges;i++){
            if(sssp[Nodes[i].from]+Nodes[i].edgeweight<sssp[Nodes[i].to]){
                cout<<"A shorter path is available"<<endl;
                return;
            }
        }
        cout<<"The SSSP is correct"<<endl;
        return ;
    }


};

int main(int argc,char** argv){
    srand(0);
    int vertex=stoi(argv[2]);
    int edge=stoi(argv[3]);
    Graph g(edge,vertex);
    string line;
    Edge E;
    fstream inputfile(argv[1],ios::in);
    for(int i=0;i<edge;i++){
        getline(inputfile,line);
        istringstream input(line);
        input>>E.from>>ws>>E.to;
        E.edgeweight = rand()%1000+1;
        g.Nodes.push_back(E);
    }
    sort(g.Nodes.begin(),g.Nodes.end(),Compare);
    g.rowcount();
    auto start = chrono::high_resolution_clock::now();
    ios_base::sync_with_stdio(false); 
    g.SSSP();
    auto end = chrono::high_resolution_clock::now();
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    time_taken *= 1e-9;
    for(auto it:g.sssp) {
        if(it==INT_MAX) cout<<"max"<<" ";
        else cout<<it<<" ";
    }
    cout<<"\n";
    cout << "Time taken by program without OPENMP library is : " << fixed 
         << time_taken << setprecision(9);
    cout << " sec" << endl;
    g.correctness();
    return 0;
}