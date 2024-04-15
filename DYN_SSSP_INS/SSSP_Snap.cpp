#include <bits\stdc++.h>
#include <omp.h>
using namespace std;

typedef struct Edge{
    int from;
    int to;
    float edgeweight;

    bool operator()(Edge e1,Edge e2){
        if(e1.edgeweight<e2.edgeweight) return true;
        return false;
    }
} edge;

bool Compare(Edge e1, Edge e2){
    return e1.from < e2.from;
}

int SSSP(vector<edge> node,vector<int> sum,int* ar){
    ar[0]=0;
    int j=0;
    double itime,ftime,etime;
    itime = omp_get_wtime();
    #pragma omp parallel private(j) shared(ar,node,sum)
    {
        #pragma omp for
        for(int k=0;k<sum.size()-1;k++){ //relaxation of vertices for vertices-1 times
            for(int i=0;i<node.size();i++){
                if(ar[node.at(i).from]+node.at(i).edgeweight<ar[node.at(i).to]) ar[node.at(i).to] = ar[node.at(i).from]+node.at(i).edgeweight;
            }
        }
    }
    ftime = omp_get_wtime();
    etime = ftime-itime;
    cout<<"Ran for "<<etime<<"s"<<endl;
    return 1;
}

void SSSPNEW(int* ar,vector<int> sum,vector<edge> node,int to){
    int i=0;
    #pragma omp parallel private(i) shared(ar,node,sum)
    {
        #pragma omp for
        for(int i=sum[to];i<sum[to+1];i++){
            if(ar[node.at(i).from]+node.at(i).edgeweight<ar[node.at(i).to]) {
                ar[node.at(i).to] = ar[node.at(i).from]+node.at(i).edgeweight;
                SSSPNEW(ar,sum,node,node.at(i).to);
            }
        }
    }
}

void DYNSSSP(int* ar,vector<int> sum,vector<edge> node,Edge E){
    if(ar[E.from]+E.edgeweight<ar[E.to]) {
        ar[E.to] = ar[E.from]+E.edgeweight;
        SSSPNEW(ar,sum,node,E.to);
    }
    
    return;
}


int main(int argc,char** argv){
    srand(0);
    omp_set_num_threads(70);
    vector<edge> node ;  //to store the crs input of the graph
    vector<int> sum;
    vector<int> position;
    int vertex=stoi(argv[2]);
    int edge=stoi(argv[3]);
    int last,count=0;
    string line;
    Edge E;
    fstream inputfile(argv[1],ios::in);
    for(int i=0;i<edge;i++){
        getline(inputfile,line);
        istringstream input(line);
        input>>E.from>>ws>>E.to;
        E.edgeweight = rand()%1000+1;
        node.push_back(E);
    }
    int ar[vertex];
    fill(ar,ar+vertex,INT_MAX);
    sort(node.begin(),node.end(),Compare);
    int value =0 ;
    for(int i=0;i<node.size();i++){
        if(node.at(i).from != last){
            while(value!=node.at(i).from){
                sum.push_back(count); 
                value++;
            }
            last = node.at(i).from;
            sum.push_back(count);
            value++;
        }
        count++;
    }
    sum.push_back(count);
    for(int i=sum.size()-1;i<vertex;i++){
        sum.push_back(count);
    }
    int output = SSSP(node,sum,ar);
    if(output){
        cout<<"The shortest distance from the starting node to all other nodes is\n"<<endl;
        for(auto it : ar) {
            if(it==INT_MAX){
                cout<<"max ";
                continue;
            }
            cout<<it<<" ";
            }
    }    
    cout<<endl<<endl;
    int newedges = stoi(argv[5]);
    vector<Edge> addedge;
    fstream inputfile1(argv[4],ios::in);
    for(int i=0;i<newedges;i++){
        getline(inputfile1,line);
        istringstream input(line);
        input>>E.from>>ws>>E.to;
        E.edgeweight = rand()%1000+1;
        addedge.push_back(E);
        node.push_back(E);
        for(int i=E.from+1;i<sum.size();i++) sum[i]++;
    }
    sort(node.begin(),node.end(),Compare);
    double itime,ftime,etime;
    itime = omp_get_wtime();
    int i=0;
    #pragma omp parallel private(i) shared(ar,node,sum)
    {
        #pragma omp for
        for(int i=0;i<newedges;i++){
            DYNSSSP(ar,sum,node,addedge.at(i));
        }
    }
    ftime = omp_get_wtime();
    etime = ftime-itime;
    cout<<"Time for one edge change "<<etime<<"s"<<endl;
    cout<<"The shortest distance from the starting node to all other nodes is\n"<<endl;
    for(auto it : ar) {
        if(it==INT_MAX){
            cout<<"max ";
            continue;
        }
        cout<<it<<" ";
        }
    return 0;
}