#include <iostream>
#include<limits.h>
#include <vector>
#include<fstream>
#include<omp.h>
using namespace std;

int SSSP(vector<int> node,vector<int> sum,int* ar){
    ar[0]=0;
    int j=0;
    int count=0;
    int times = 0;
    double itime,ftime,etime;
    itime = omp_get_wtime();
    #pragma omp parallel private(j,times) shared(ar,node,sum) reduction(+:count)
    {
        #pragma omp for
        for(int k=0;k<sum.size()-2;k++){ //relaxation of vertices for vertices-1 times
        // #pragma omp parallel for
        j=0;
        for (auto i = 0; i < sum.size()-1; i++){  //passing through all the edges
            #pragma omp critical 
            times = sum[count+1]-sum[count];
            while(times>0){ //Bellman_ford algorithm
                if(ar[i]+node.at(j)<ar[node.at(j+1)]) ar[node.at(j+1)] = ar[i]+node.at(j);
                j+=2;times--;
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
                if(ar[i]+node.at(j)<ar[node.at(j+1)]) {
                    cout<<"There is a negative cycle in the graph";
                    return 0;}
                j+=2;times--;
            }
            count++;
    }
    return 1;
}

int main(int argc,char** argv){
    vector<int> node ;  //to store the crs input of the graph
    vector<int> sum = {0};
    fstream inputfile(argv[1],ios::in);
    int edge,vertex,push=0;
    inputfile>>vertex>>edge;
    for(int i=0;i<2*edge;i++){
        inputfile>>push;
        node.push_back(push);
    }
    for(int i=0;i<vertex;i++){
        inputfile>>push;
        sum.push_back(push);
    }
    int ar[vertex];
    fill(ar,ar+vertex,INT_MAX);
    int error = SSSP(node,sum,ar);
    for(auto it:node) cout<<it<<" ";
    cout<<endl;
    for(auto it:sum) cout<<it<<" ";
    cout<<endl;
    if(error){
        cout<<"The shortest distance from the starting node to all other nodes is\n"<<endl;
        for(auto it : ar) cout<<it<<" ";
    }
    
    cout<<endl;
    return 0;
}