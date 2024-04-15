#include<iostream>
#include<vector>
#include<fstream>
#include<omp.h>
#include<queue>
#include<chrono>


using namespace std;

vector<int> BFS(vector<int> node,vector<int> sum){    
    vector<bool> visited(sum.size(),false);
    queue<int> q;
    int current=0;
    vector<int> bfs;
    int no=1;
    #pragma omp parallel
    for(int i=0;i<sum.size()-1;i++){
        if(!visited[i]){
            visited[i] = true;
            q.push(i);
            bfs.clear();
            while(!q.empty()){
                current = q.front();
                q.pop();
                bfs.push_back(current);
                #pragma omp critical
                for(int i = sum[current];i<sum[current+1];i++){
                    if(!visited[node[2*i+1]]){
                        visited[node[2*i+1]] = true;
                        q.push(node[2*i+1]);
                    }
                }
            }
        cout<<"connected component "<<no++<<" the nodes are visited in the order : ";
        for(auto it:bfs) cout<<it<<" ";
        cout<<endl;
        }
    }
    return bfs;
}
    
int main(int argc,char** argv){
    vector<int> node ;  //to store the crs input of the graph
    vector<int> sum = {0};
    fstream inputfile(argv[1],ios::in);
    int edge,vertex,push=0;
    inputfile>>edge>>vertex;
    for(int i=0;i<2*edge;i++){
        inputfile>>push;
        node.push_back(push);
    }
    for(int i=0;i<vertex;i++){
        inputfile>>push;
        sum.push_back(push);
    }
    // auto begin = std::chrono::steady_clock::now();
    // auto start = omp_get_wtime();
    vector<int> output = BFS(node,sum);
    // auto end = omp_get_wtime();
    // std::cout << "Time difference = " << (end - start) * 1e3 << "[ms]" << std::endl;
    // auto end = std::chrono::steady_clock::now();
    // std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    // std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
    // for(auto it :output) cout<<it<<" ";
    // cout<<endl;
    return 0;
}