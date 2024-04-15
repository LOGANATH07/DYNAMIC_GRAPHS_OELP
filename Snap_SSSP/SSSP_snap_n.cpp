#include <iostream>
#include <fstream>
#include <limits.h>
#include <vector>
#include<sstream>
#include <omp.h>
#include <algorithm>
#include<queue>
#include<random>
#include <chrono>
#include <bits/stdc++.h>

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

int SSSP(vector<edge> node,vector<int> sum,int* ar){
    ar[0]=0;
    int j=0;
    int count=0;
    int times = 0;

        j=0;
        for(int k=0;k<sum.size()-2;k++){ //relaxation of vertices for vertices-1 times
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
    //to check for negative cycles
    for (auto i = 0; i < sum.size()-1; i++){ 
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

    srand(0);
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
    int ar[vertex];
    fill(ar,ar+vertex,INT_MAX);
    // for (size_t i = 0; i < position.size(); ++i) {
    //     std::cout << position[i] << std::endl;
    // }
    auto start = chrono::high_resolution_clock::now();
    ios_base::sync_with_stdio(false);    
    int output = SSSP(node,sum,ar);
    auto end = chrono::high_resolution_clock::now();
 
    // Calculating total time taken by the program.
    double time_taken = 
      chrono::duration_cast<chrono::nanoseconds>(end - start).count();
 
    time_taken *= 1e-9;
 
    cout << "Time taken by program without OPENMP library is : " << fixed 
         << time_taken << setprecision(9);
    cout << " sec" << endl;
    if(output){
        cout<<"The shortest distance from the starting node to all other nodes is\n"<<endl;
        for(auto it : ar) cout<<it<<" ";
    }
    
    cout<<endl;
    return 0;
}