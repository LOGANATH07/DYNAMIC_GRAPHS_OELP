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

int SSSP(vector<edge> node,vector<int> sum,double* ar){
    ar[0]=0;
    int j=0;
    int count=0;
    int times = 0;
    double itime,ftime,etime;
    itime = omp_get_wtime();
    #pragma omp parallel private(j,times) shared(ar,node,sum) reduction(+:count)
    {
        j=0;
        #pragma omp for
        for(int k=0;k<sum.size()-2;k++){ //relaxation of vertices for vertices-1 times
            // #pragma omp for
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
    }
    ftime = omp_get_wtime();
    etime = ftime-itime;
    cout<<"Ran for "<<etime<<"s"<<endl;
    // to check for negative cycles
    // for (auto i = 0; i < sum.size()-1; i++){ 
    //         // #pragma omp critical 
    //         times = sum[count+1]-sum[count];
    //         while(times>0){
    //             if(ar[i]+node.at(j).edgeweight<ar[node.at(j).to]) {
    //                 cout<<"There is a negative cycle in the graph";
    //                 return 0;}
    //             j+=1;times--;
    //         }
    //         count++;
    // }
    return 1;
}

int main(int argc,char** argv){
    srand(0);
    omp_set_num_threads(70);
    vector<edge> node ;  //to store the crs input of the graph
    vector<int> sum;
    vector<int> position;
    int vertex=stoi(argv[2]);
    int edge=stoi(argv[3]);
    int push,last,count=0;
    string line;
    Edge E;
    fstream inputfile(argv[1],ios::in);
    for(int i=0;i<edge;i++){
        getline(inputfile,line);
        istringstream input(line);
        input>>E.from>>ws>>E.to;
        E.edgeweight = rand()%1000+1;
        push=E.from;
        node.push_back(E);
        if(push!=last){
            last=push;
            sum.push_back(count);
            position.push_back(last);
        }
        count++;
    }
    sum.push_back(count);
    double ar[vertex];
    fill(ar,ar+vertex,INT_MAX);
    // for(auto it:node) cout<<it.to<<endl;
    // for (size_t i = 0; i < position.size(); ++i) {
    //     std::cout << position[i] << std::endl;
    // }
    sort(node.begin(),node.end(),Compare);
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
    
    cout<<endl;
    return 0;
}