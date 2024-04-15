#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

typedef struct Edge{
    int from;
    int to;
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
    Edge start = {0,0,0};
    priority_queue<Edge,vector<Edge>,Edge> q;
    int current=0;
    int next=0;
    vector<pair<int,int>> bfs={};
    visited[0] = true;
    q.push(start);
    double itime,ftime,etime;
    itime = omp_get_wtime();
    while(!q.empty()){
        current = pos(position,q.top().to);
        q.pop();
        pair<int,int> topush = {node[current].from,node[current].to};
        bfs.push_back(topush);
        #pragma omp parallel private(next) shared(node,sum)
        {
            next=0;
            #pragma omp parallel
            for(int i = sum[current];i<sum[current+1];i++){ 
                next = pos(position,node[i].to);
                #pragma omp critical
                if(!visited[next]){
                    visited[next] = true;
                    q.push(node[i]);
                }
            }
        }
    }
    ftime = omp_get_wtime();
    etime = ftime-itime;
    cout<<"Ran for "<<etime<<"s"<<endl;
    for(auto it:bfs) cout<<" "<<it.second<<" ";
    cout<<endl;
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
    // for (size_t i = 0; i < position.size(); ++i) {
    //     std::cout << position[i] << std::endl;
    // }
    vector<pair<int,int>> output = BFS(node,sum,position);
    return 0;
}