#include <iostream>
#include <fstream>
#include <limits.h>
#include <vector>
#include<sstream>
#include <omp.h>
#include <algorithm>
#include<queue>
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

int SSSP(vector<edge> node,vector<int> sum,double* ar){
    ar[0]=0;
    int j=0;
    int count=0;
    int times = 0;
    double itime,ftime,etime;
    itime = omp_get_wtime();
    // #pragma omp parallel for
    #pragma omp parallel private(j,times) shared(ar,node,sum) reduction(+:count)
    {
        j=0;
        #pragma omp for
        for(int k=0;k<sum.size()-2;k++){ //relaxation of vertices for vertices-1 times
            // #pragma omp parallel for
            for (auto i = 0; i < sum.size()-1; i++){  //passing through all the edges
                // #pragma omp critical 
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
    //to check for negative cycles
    for (auto i = 0; i < sum.size()-1; i++){ 
            // #pragma omp critical 
            times = sum[count+1]-sum[count];
            while(times>0){
                if(ar[i]+node.at(j).edgeweight<ar[node.at(j).to]) {
                    cout<<"There is a negative cycle in the graph";
                    return 0;}
                j+=1;times--;
            }
            count++;
    }
    return 1;
}

int main(int argc,char** argv){
    vector<edge> node ;  //to store the crs input of the graph
    vector<int> sum;
    vector<int> position;
    int vertex=475;
    int edge=13289;
    int push,last,count=0;
    string line;
    Edge E;
    fstream inputfile(argv[1],ios::in);
    for(int i=0;i<edge;i++){
        getline(inputfile,line);
        istringstream input(line);
        input>>E.from>>ws>>E.to;
        input.seekg(12,ios::cur);
        input>>E.edgeweight;
        input.seekg(1,ios::cur);
        // cout<<E.edgeweight<<" "<<E.node<<" "<<push<<endl;
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
    // for (size_t i = 0; i < position.size(); ++i) {
    //     std::cout << position[i] << std::endl;
    // }
    int output = SSSP(node,sum,ar);
    if(output){
        cout<<"The shortest distance from the starting node to all other nodes is\n"<<endl;
        for(auto it : ar) cout<<it<<" ";
    }
    
    cout<<endl;
    return 0;
}