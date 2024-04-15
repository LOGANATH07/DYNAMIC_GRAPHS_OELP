#include<bits/stdc++.h>
using namespace std;

typedef struct Edge
{
    int edgeweight;
    int to;
} edge;

bool Compare(Edge e1,Edge e2){
    return (e1.from < e2.from);
}

int main(int argc,char** argv){
    srand(3);
    vector<edge> node ;   //to store the  input  graph in the CSR(compressed sparse row) format
    vector<int> sum;
    int push,last,count=0;
    int vertex=stoi(argv[2]);
    int edge=stoi(argv[3]);
    string line;
    Edge E;
    fstream inputfile(argv[1],ios::in);
    for(int i=0;i<edge;i++){
        getline(inputfile,line);
        istringstream input(line);
        input>>push>>ws>>E.to;
        E.edgeweight = rand()%1000+1;
        node.push_back(E);
        if(push!=last){//source vertex is changing then update sum for source vertex.
            last=push;//last is also vertex
            sum.push_back(count);
        }
        count++;
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