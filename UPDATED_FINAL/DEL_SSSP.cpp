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

    void SSSPDEL(int fnode,int oldedge){
        for(int i=isum[fnode];i<isum[fnode+1];i++){
            if(oldedge+Nodes[i].edgeweight==sssp[Nodes[i].to]){
                int newedge = sssp[Nodes[i].to];
                sssp[Nodes[i].to] = INT_MAX;
                for(int j=0;j<Edges;j++){
                    if( Nodes[j].to == Nodes[i].to && sssp[Nodes[j].from]+Nodes[j].edgeweight<sssp[Nodes[i].to]){
                        sssp[Nodes[i].to] =sssp[Nodes[j].from]+Nodes[j].edgeweight;
                    }
                }
                SSSPDEL(Nodes[i].to,newedge);
            }
        }
    }

    void DYNDEL(edge E) {
        int oldedge = sssp[E.to];
        // cout<<E.from<<" "<<E.to<<" "<<isum[E.from+1]-isum[E.from]<<"\n";
        for(int i=isum[E.from];i<isum[E.from+1];i++){
            // cout<<Nodes[i].from<<" "<<Nodes[i].to<<" "<<Nodes.size()<<"\n";
            if(Nodes[i].to==E.to){
                Nodes.erase(Nodes.begin()+i);
                Edges--;
                for(int j = E.from;j<isum.size();j++){
                   isum[j]--;
                }
                break;
            }
            // cout<<Nodes[i].from<<" "<<Nodes[i].to<<" "<<Nodes.size()<<"\n";
        }
        if(sssp[E.from] + E.edgeweight != sssp[E.to]) return;
        sssp[E.to] = INT_MAX;
        for(int i=0;i<Edges;i++){
            if(Nodes[i].to == E.to){
                if(sssp[Nodes[i].from]+Nodes[i].edgeweight<sssp[Nodes[i].to]){
                    // if(Nodes[i].to==0) continue;
                    sssp[Nodes[i].to] = sssp[Nodes[i].from]+Nodes[i].edgeweight;
                }
            }
        }  
        correctness();  
        SSSPDEL(E.to,oldedge);
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
                DYNDEL(newedges->at(i));
            }
        }
        ftime = omp_get_wtime();
        etime = ftime-itime;
        cout<<"Time for one edge change "<<etime/newedge<<"s\n";
    }

    void correctness(){
        for(int i=0;i<Edges;i++){
            if(sssp[Nodes[i].from]+Nodes[i].edgeweight<sssp[Nodes[i].to]){
                // cout<<Nodes[i].from<<" "<<Nodes[i].to<<"\n";
                // cout<<sssp[Nodes[i].from]<<" "<<Nodes[i].edgeweight<<" "<<sssp[Nodes[i].to];
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
    g.correctness();
    int newedges = stoi(argv[4]);
    vector<Edge> addedge;
    for(int i=0;i<newedges;i++){
        Edge random = g.Nodes.at(rand()%vertex);
        addedge.push_back(random);
    }
    g.DYNGRAPH(newedges,&addedge);
    for(auto it:g.sssp) {
        if(it==INT_MAX) cout<<"max"<<" ";
        else cout<<it<<" ";
    }
    cout<<"\n";
    g.correctness();
    return 0;
}