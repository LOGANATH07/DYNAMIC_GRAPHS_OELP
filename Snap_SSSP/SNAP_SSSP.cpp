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

int SSSP(vector<edge> node,vector<int> sum,int* ar){
    ar[0]=0;
    int j=0;int count=0;int times=0;
    #pragma omp parallel private(j,times) shared(ar,node,sum) reduction(+:count)
    {
        j=0;
        #pragma omp for
        for (auto i = 0; i < sum.size()-1; i++){  //passing through all the edges
                times = sum[count+1]-sum[count];
                while(times>0 && j<13289){ //Bellman_ford algorithm
                    if(ar[i]+node.at(j).edgeweight<ar[node.at(j).to]) ar[node.at(j).to] = ar[i]+node.at(j).edgeweight;
                    j+=1;times--;
                }
                count++;
            }
            j=0;count=0;
    }
    // for (auto i = 0; i < node.size()-1; i++){  //checking for negative cycles
    //         if(ar[node.at(i).from]+node.at(i).edgeweight<ar[node.at(i).to]) return 0;
    //     }
    return 1;
}

int main(int argc,char** argv){
    srand(3);
    vector<edge> node ;   //to store the  input  graph in the CSR(compressed sparse row) format
    vector<int> sum;
    int last,push,count=0;
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
        push=E.from;
        node.push_back(E);
        if(push!=last){
            last=push;
            sum.push_back(count);
        }
        count++;
    }
    sort(node.begin(),node.end(),Compare);//sorted using custom comparator
    int ar[vertex];
    fill(ar,ar+vertex,INT_MAX);
    int output = SSSP(node,sum,ar);
    if(output){
        cout<<"The shortest distance from the starting node to all other nodes is\n"<<endl;
        for(auto it : ar) cout<<it<<" ";
    }
    else cout<<"There is a negative cycle in the graph"<<endl;    
    return 0;
}