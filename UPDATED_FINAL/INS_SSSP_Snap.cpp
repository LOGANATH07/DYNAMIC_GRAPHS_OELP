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
    int Vertics=0;
    int depth = 0;

    public:

    int Edges =0;
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
        double itime,ftime,etime;
        itime = omp_get_wtime();
        bool updated = false;
        while(1){ //relaxation of vertices for vertices-1 times
            updated = false;
            #pragma omp parallel for
            for(int i=0;i<Edges;i++){
                if(sssp[Nodes[i].from]+Nodes[i].edgeweight<sssp[Nodes[i].to]) {
                    sssp[Nodes[i].to] = sssp[Nodes[i].from]+Nodes[i].edgeweight;
                    updated = true;
                }
            }
            if(!updated) break;
        }
        ftime = omp_get_wtime();
        etime = ftime-itime;
        cout<<"Ran for "<<etime<<"s\n";
    }

    void SSSPINS(int fnode){
        for(int i=isum[fnode];i<isum[fnode+1];i++){
            if(sssp[fnode]+Nodes[i].edgeweight<sssp[Nodes[i].to]){
                sssp[Nodes[i].to] = sssp[fnode]+Nodes[i].edgeweight;
                SSSPINS(Nodes[i].to);
            }
        }
    }

    void DYNINS(edge E) {
        if(sssp[E.from] + E.edgeweight < sssp[E.to]){
            sssp[E.to] = sssp[E.from] + E.edgeweight;
            SSSPINS(E.to);
        }
        return; 
    }

    void DYNGRAPH(int newedge,vector<edge>* newedges){
        double itime,ftime,etime;
        itime = omp_get_wtime();
        int i=0;
        #pragma omp parallel private(i) shared(isum,newedges,sssp)
        {
            #pragma omp for
            for(int i=0;i<newedge;i++){
                // Nodes.insert(Nodes.begin()+isum[newedges->at(i).from],newedges->at(i));
                // Edges++;
                // for(int i = newedges->at(i).from+1;i<isum.size();i++) isum[i]++;
                DYNINS(newedges->at(i));
            }
        }
        ftime = omp_get_wtime();
        etime = ftime-itime;
        cout<<"Time for one edge change "<<etime<<"s\n";
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
        E.edgeweight = rand()%1000+1;
        g.Nodes.push_back(E);
    }
    sort(g.Nodes.begin(),g.Nodes.end(),Compare);
    g.rowcount();
    g.SSSP();
    // for(auto it:g.bfs) {
    //     if(it==INT_MAX) cout<<"max"<<" ";
    //     else cout<<it<<" ";
    // }
    int newedges = stoi(argv[5]);
    vector<Edge> addedge;
    fstream inputfile1(argv[4],ios::in);
    int i=0;
    // #pragma omp parallel private(i) shared(addedge)
    // {
    //     #pragma omp for
        for(i=0;i<newedges;i++){
            getline(inputfile1,line);
            istringstream input(line);
            input>>E.from>>ws>>E.to;
            E.edgeweight = rand()%1000+1;
            addedge.push_back(E);
            g.Nodes.insert(g.Nodes.begin()+g.isum[E.from],E);
            g.Edges++;
            // cout<<E.from<<" "<<g.Nodes[g.isum[E.from]].from<<"\n";
            for(int i=E.from+1;i<g.isum.size();i++) {
                g.isum[i]++;
            }
        }
    // }
    // cout<<"later";
    // g.rowcount();
    // sort(g.Nodes.begin(),g.Nodes.end(),Compare);
    g.DYNGRAPH(newedges,&addedge);
    cout<<"\n";
    g.correctness();
    return 0;
}