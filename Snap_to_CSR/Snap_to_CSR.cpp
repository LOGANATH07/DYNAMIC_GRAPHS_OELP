#include <iostream>
#include <fstream>
#include <vector>
#include<sstream>
#include <omp.h>
#include <algorithm>
#include<queue>
using namespace std;

typedef struct Edge{
    int node;
    float edgeweight;

    bool operator()(Edge e1,Edge e2){
        if(e1.edgeweight>e2.edgeweight) return true;
        return false;
    }
} edge;

int pos(vector<int> position,int value){
    int i=0;
    while(position.at(i)!= value) i++;
    return i;
}

vector<pair<int,int>> BFS(vector<Edge> node,vector<int> sum,vector<int> position){    
    vector<bool> visited(sum.size(),false);
    priority_queue<Edge,vector<Edge>,Edge> p_q;
    int current=0;
    vector<pair<int,int>> bfs;
    // visited[0] = true;
    int no=1;
    for(int i=0;i<sum.size()-1;i++){
        int index = pos(position,node.at(i).node);
        if(!visited[index]){
            // cout<<position.at(index)<<endl;
            visited[index] = true;
            p_q.push(node.at(index));
            pair<int,int> bfspush = {position.at(index),0};
            bfs.push_back(bfspush);
            while(!p_q.empty()){
                current = pos(position,p_q.top().node);
                p_q.pop();
                pair<int,int> bfspush2 = {position.at(current),0};
                bfs.push_back(bfspush2);
                for(int i = sum[current];i<sum[current+1];i++){
                    if(!visited[node.at(i).node]){
                        visited[node.at(i).node] = true;
                        p_q.push(node.at(i));
                    }
                }
            }
            cout<<"connected component "<<no++<<" the nodes are visited in the order : \n";
            for(auto it:bfs) cout<<it.first<<" "<<it.second<<endl;
            cout<<endl<<endl;
        }
        bfs.clear();
    }
    // int temp=0;
    // for(auto it : visited){
    //     if(it==0) cout<<position.at(temp)<<endl;
    //     temp++;
    // }
    return bfs;
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
        input>>push>>ws>>E.node;
        input.seekg(12,ios::cur);
        input>>E.edgeweight;
        input.seekg(1,ios::cur);
        // cout<<E.edgeweight<<" "<<E.node<<" "<<push<<endl;
        node.push_back(E);
        if(push!=last){
            last=push;
            sum.push_back(count);
            position.push_back(push);
        }
        count++;
    }
    sum.push_back(count);
    // for (size_t i = 0; i < position.size(); ++i) {
    //     std::cout << position[i] << std::endl;
    // }
    vector<pair<int,int>> output = BFS(node,sum,position);
    return 0;
}