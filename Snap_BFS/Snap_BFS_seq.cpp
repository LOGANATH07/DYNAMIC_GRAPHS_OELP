#include<bits/stdc++.h>
// #include<omp.h>

typedef struct Edge{
    int from;
    int to;
    float edgeweight;
} edge;

typedef struct{
    bool operator()(Edge e1,Edge e2){
        return e1.edgeweight < e2.edgeweight;
    }
} prioritycomp;

using namespace std;

bool Compare(Edge e1, Edge e2){
    return e1.from < e2.from;
}

void BFS(vector<edge> node,vector<int> sum,vector<int> bfs){
    vector<bool> visited(sum.size()-1,false);
    priority_queue<edge,vector<edge>,prioritycomp> queue;
    Edge start = {0,0,0};
    queue.push(start);
    int current,neighbour=0;
    int components=1;
    // double itime,ftime,etime;
    // itime = omp_get_wtime();
    // #pragma parallel private(current,neighbours) shared(queue,bfs)
    {
        current=0;
        neighbour=0;
        // #pragma omp for
        for(int j=0;j<sum.size()-1;j++)
        {
            if(visited[j]==false){
                visited[j] = true;
                if(j!=0){
                    bfs.push_back(j);
                    queue.push(node.at(sum[j]));
                    // cout<<node.at(sum[j]).from<<endl;
                }
                while(!queue.empty()){
                    current = queue.top().to;
                    queue.pop();
                    bfs.push_back(current);
                    for(int i = sum[current];i<sum[current+1];i++){
                        neighbour = node.at(i).to; 
                        // cout<<current<<" "<<node.at(i).edgeweight<<" "<<neighbour<<endl;
                        if(!visited[neighbour]){
                            visited[neighbour] = true;
                            queue.push(node.at(i));
                        }
                    }
                }
                cout<<"Connected component "<<components++<<endl;
                for(auto it:bfs) cout<<it<<" ";
                cout<<endl<<endl;
                bfs.clear();
            }
        }
    }
    // ftime = omp_get_wtime();
    // etime = ftime-itime;
    // cout<<"Ran for "<<etime<<"s"<<endl;
    // sort(bfs.begin(),bfs.end());
    // for(auto it : bfs) fprintf(stderr,"%d ",it);
    // int samp=0;
    // for(auto i=visited.begin();i!=visited.end();i++){
    //     if(*i==false){
    //         fprintf(stderr,"%d\n",samp);
    //     }
    //     samp++;
    // }
    return;
}

int main(int argc,char** argv){
    srand(3);
    vector<edge> node;
    vector<int> sum={0};
    vector<int> output;
    int vertex=stoi(argv[2]); //475
    int edge=stoi(argv[3]);  //13289
    int push=0,last=0,count=0;
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
    sort(node.begin(),node.end(),Compare);
    for(int i=0;i<edge;i++){
        if(node.at(i).from != last){
            last = node.at(i).from;
            sum.push_back(count);
        }
        count++;
    }
    sum.push_back(count);
    auto start = chrono::high_resolution_clock::now();
    BFS(node,sum,output);
    auto end = chrono::high_resolution_clock::now();
     double time_taken = 
      chrono::duration_cast<chrono::nanoseconds>(end - start).count();
 
    time_taken *= 1e-9;
    cout << "Time taken by program is : " << fixed 
         << time_taken << setprecision(9);
    cout << " sec" << endl;
    // cout<<endl;
    return 0;
}