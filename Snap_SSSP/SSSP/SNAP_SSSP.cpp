#include <bits/stdc++.h>

using namespace std;

typedef struct Edge{
    int from;
    int to;
    float edgeweight;
} edge;

bool Compare(Edge e1,Edge e2){
    return (e1.from < e2.from);
}

int SSSP(vector<edge> node,int vertex,int* ar){
    ar[0]=0;
    for(int k=0;k<vertex-2;k++){ //relaxation of vertices for vertices-1 times
        for (auto i = 0; i < node.size()-1; i++){  //passing through all the edges
            if(ar[node.at(i).from]+node.at(i).edgeweight<ar[node.at(i).to]) ar[node.at(i).to] = ar[node.at(i).from]+node.at(i).edgeweight;
        }
    }
    for (auto i = 0; i < node.size()-1; i++){  //checking for negative cycles
            if(ar[node.at(i).from]+node.at(i).edgeweight<ar[node.at(i).to]) return 0;
        }
    return 1;
}

int main(int argc,char** argv){
    srand(3);
    vector<edge> node ;   //to store the  input  graph in the CSR(compressed sparse row) format
    int vertex=stoi(argv[2]);
    int edge=stoi(argv[3]);
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
    sort(node.begin(),node.end(),Compare);//sorted using custom comparator
    int ar[vertex];
    fill(ar,ar+vertex,INT_MAX);
    int output = SSSP(node,vertex,ar);
    if(output){
        cout<<"The shortest distance from the starting node to all other nodes is\n"<<endl;
        for(auto it : ar) cout<<it<<" ";
    }
    else cout<<"There is a negative cycle in the graph"<<endl;    
    return 0;
}