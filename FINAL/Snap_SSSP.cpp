#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

struct Edge{
    int from;
    int to;
    float edgeweight;

    bool operator<(const Edge& e2) const {
        return from < e2.from;
    }
};

bool Compare(Edge e1, Edge e2){
    return e1.from < e2.from;
}

void SSSP(vector<Edge>& node, vector<int>& sum, vector<int>& ar){
    ar[0]=0;
    double itime,ftime,etime;
    itime = omp_get_wtime();
    bool updated = false;
    while(1){ //relaxation of vertices for vertices-1 times
        updated = false;
        #pragma omp parallel for
        for(int i=0;i<node.size();i++){
            if(ar[node.at(i).from]+node.at(i).edgeweight<ar[node.at(i).to]) {
                ar[node.at(i).to] = ar[node.at(i).from]+node.at(i).edgeweight;
                updated = true;
            }
        }
        if(!updated) break;
    }
    ftime = omp_get_wtime();
    etime = ftime-itime;
    cout<<"Ran for "<<etime<<"s\n";
}

int main(int argc,char** argv){
    srand(0);
    omp_set_num_threads(70);
    vector<Edge> node ;  //to store the crs input of the graph
    vector<int> sum;
    vector<int> position;
    int vertex=stoi(argv[2]);
    int edge=stoi(argv[3]);
    int last,count=0;
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
    vector<int> ar(vertex, INT_MAX);
    sort(node.begin(),node.end(),Compare);
    int value =0 ;
    for(int i=0;i<node.size();i++){
        if(node.at(i).from != last){
            while(value!=node.at(i).from){
                sum.push_back(count); 
                value++;
            }
            last = node.at(i).from;
            sum.push_back(count);
            value++;
        }
        count++;
    }
    sum.push_back(count);
    for(int i=sum.size()-1;i<vertex;i++){
        sum.push_back(count);
    }
    SSSP(node,sum,ar);
    // cout<<"The shortest distance from the starting node to all other nodes is\n";
    // for(auto it : ar) {
    //     if(it==INT_MAX){
    //         cout<<"max ";
    //         continue;
    //     }
    //     cout<<it<<" ";
    // }    
    // cout<<"\n\n";
    // correctness(node,sum,ar);

    return 0;
}