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
    vector<int> bfs;
    vector<int> isum;
    vector<bool> visited;
    Graph(int edges,int vertices):
    bfs(vertices,INT_MAX),visited(vertices,false),isum(vertices+1,0)
    {
        bfs[0] = 0;
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

    void BFS(){
        double itime,ftime,etime;
        itime = omp_get_wtime();
        bool updated = false;
        while(1){ //relaxation of vertices for vertices-1 times
            updated = false;
            #pragma omp parallel for
            for(int i=0;i<Edges;i++){
                if(bfs[Nodes[i].from]+Nodes[i].edgeweight<bfs[Nodes[i].to]) {
                    bfs[Nodes[i].to] = bfs[Nodes[i].from]+Nodes[i].edgeweight;
                    updated = true;
                }
            }
            if(!updated) break;
        }
        ftime = omp_get_wtime();
        etime = ftime-itime;
        cout<<"Ran for "<<etime<<"s\n";
    }

    // void BFS(){
    //     double itime,ftime,etime;
    //     itime = omp_get_wtime();
    //     // while(1){
    //     //     updated = false;
    //         int fnode,tnode = 0;
    //         queue<int> Q;
    //         Q.push(0);
    //             // #pragma omp parallel for
    //             // for(int i=0;i<Edges;i++){
    //             //     if(bfs[Nodes[i].from]==depth){
    //         while(!Q.empty()){
    //             fnode = Q.front();
    //             Q.pop();
    //             // #pragma omp parallel private(fnode,tnode) shared(Nodes,isum)
    //             // {   
    //             #pragma omp parallel for
    //             for(int j=isum[fnode];j<isum[fnode+1];j++){
    //                 tnode = Nodes[j].to;
    //                 if(bfs[tnode]>depth+1){
    //                     Q.push(tnode);
    //                     bfs[tnode] = depth+1;
    //                     // updated = true;
    //                 }
    //             }
    //             // }
    //             depth++;
    //             //     }
    //             // }
    //         }
    //     //     if(!updated) break; 
    //     // }
    //     ftime = omp_get_wtime();
    //     etime = ftime-itime;
    //     cout<<"Ran for "<<etime<<"s"<<endl;
    // }


};

int main(int argc,char** argv){
    srand(0);
    omp_set_num_threads(16);
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
        E.edgeweight = 1;
        g.Nodes.push_back(E);
    }
    sort(g.Nodes.begin(),g.Nodes.end(),Compare);
    g.rowcount();
    // g.BFS();
    g.BFS();
    for(auto it:g.bfs) {
        if(it==INT_MAX) cout<<"max"<<" ";
        else cout<<it<<" ";
    }
    cout<<"\n";
    return 0;
}