#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

struct Edge {
    int from;
    int to;
    float edgeweight;

    bool operator<(const Edge& other) const {
        return edgeweight < other.edgeweight;
    }

    bool operator==(const Edge& other) const {
        return from == other.from && to == other.to && edgeweight == other.edgeweight;         ////
    }
};

bool Compare(const Edge& e1, const Edge& e2) {
    return e1.from < e2.from;
}

void SSSP(vector<Edge>& node, vector<int>& sum, vector<int>& ar) {
    ar[0] = 0;
    #pragma omp parallel for
    for (int k = 0; k < sum.size() - 1; k++) {
        for (int i = 0; i < node.size(); i++) {
            if (ar[node.at(i).from] + node.at(i).edgeweight < ar[node.at(i).to]) {
                ar[node.at(i).to] = ar[node.at(i).from] + node.at(i).edgeweight;
            }
        }
    }
}

void SSSPNEW(vector<int>& ar, vector<int>& sum, vector<Edge>& node, int to, int oldedge) {
    #pragma omp parallel for
    for (int i = sum[to]; i < sum[to + 1]; i++) {
        Edge& next = node[i];
        if (oldedge + next.edgeweight == ar[next.to]) {
            int proedge = ar[next.to];
            for(int j=0;j<node.size();j++){
                if(node.at(j).from==0){
                    // cout<<node.at(j).from<<" "<<node.at(j).to<<"\n";
                    // cout<<ar[node.at(j).from]<<" "<<node.at(j).edgeweight<<"\n";
                    if( node.at(j).to == node.at(i).to && ar[node.at(j).from]+node.at(j).edgeweight<ar[node.at(i).to]){
                        ar[node.at(i).to] = ar[node.at(j).from]+node.at(j).edgeweight;
                    }
                    // cout<<ar[node.at(j).to]<<"\n";
                }
            }
            SSSPNEW(ar, sum, node, next.to, proedge);
        }
    }
}

void DYNSSSPDEL(vector<int>& ar,vector<int> sum,vector<Edge> node,Edge E){
    int oldedge = ar[E.to];
    for(int i=sum[E.from];i<sum[E.from+1];i++){
        if(node.at(i).to==E.to){
            // cout<<E.from<<" "<<E.to<<"\n";
            // cout<<node.at(i).from<<" "<<node.at(i).to<<"\n";
            node.erase(remove(node.begin(), node.end(), E), node.end());                     ////
            // cout<<node.at(i).from<<" "<<node.at(i).to<<"\n";
            break;
        }
    }
    for(int j = E.from;j<sum.size();j++){
        sum[j]--;
    }
    if(ar[E.from] + E.edgeweight != ar[E.to]) return;
    // ar[E.to] = INT_MAX;
    for(int i=0;i<node.size();i++){
        if(node.at(i).to == E.to && ar[node.at(i).from]+node.at(i).edgeweight<ar[node.at(i).to]){
            if(node.at(i).from==0){
                // cout<<node.at(i).from<<" "<<node.at(i).to<<"\n";
                // cout<<ar[node.at(i).from]<<" "<<node.at(i).edgeweight<<"\n";
                ar[E.to] = ar[node.at(i).from]+node.at(i).edgeweight;
                // cout<<ar[E.to]<<"\n";
            }
        }
    }    
    // SSSPNEW(ar,sum,node,E.to,oldedge);
    return;
}

void correctness(vector<Edge>& node, vector<int>& sum, vector<int>& ar){
    for(int i=0;i<node.size();i++){
        if(ar[node.at(i).from]+node.at(i).edgeweight<ar[node.at(i).to]){
            cout<<node.at(i).from<<" "<<node.at(i).to<<"\n";
            cout<<ar[node.at(i).from]<<" "<<node.at(i).edgeweight<<"\n";
            cout<<ar[node.at(i).to]<<"\n";
            cout<<"A shorter path is available"<<endl;
            return;
        }
    }
    cout<<"The SSSP is correct"<<endl;
    return ;
}


int main(int argc,char* argv[]){                         ////
    srand(0);
    omp_set_num_threads(12);
    int edges=stoi(argv[3]);
    int vertex=stoi(argv[2]);
    vector<Edge> node ;  //to store the crs input of the graph
    node.reserve(edges);
    vector<int> sum;
    sum.reserve(vertex + 1);                              ////
    int last,count=0;
    string line;
    Edge E;
    fstream inputfile(argv[1],ios::in);
    for(int i=0;i<edges;i++){
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
    cout<<"The shortest distance from the starting node to all other nodes is\n"<<endl;
    for(auto it : ar) {
        if(it==INT_MAX){
            cout<<"max ";
            continue;
        }
        cout<<it<<" ";
        }  
    cout<<endl<<endl;
    correctness(node,sum,ar);
    srand((unsigned) time(NULL));
    int newedges = stoi(argv[4]);
    vector<Edge> addedge;
    // fstream inputfile1(argv[4],ios::in);
    for(int i=0;i<newedges;i++){
        Edge random = node.at(rand()%88234);
        addedge.push_back(random);

    }
    double itime,ftime,etime;
    itime = omp_get_wtime();
    int i=0;
    #pragma omp parallel private(i) shared(ar,node,sum)
    {
        #pragma omp for
        for(int i=0;i<newedges;i++){
                DYNSSSPDEL(ar,sum,node,addedge.at(i));
        }
    }
    ftime = omp_get_wtime();
    etime = ftime-itime;
    cout<<"Time for one edge change "<<etime<<"s"<<endl;
    cout<<"The shortest distance from the starting node to all other nodes is\n"<<endl;
    for(auto it : ar) {
        if(it==INT_MAX){
            cout<<"max ";
            continue;
        }
        cout<<it<<" ";
        }
    cout<<"\n\n";
    correctness(node,sum,ar);
    return 0;
}