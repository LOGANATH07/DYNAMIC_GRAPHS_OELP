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

int SSSP(vector<edge> node,vector<int> sum,int* ar){
    ar[0]=0;
    int j=0;
    double itime,ftime,etime;
    itime = omp_get_wtime();
    #pragma omp parallel private(j) shared(ar,node,sum)
    {
        #pragma omp for
        for(int k=0;k<sum.size()-1;k++){ //relaxation of vertices for vertices-1 times
            for(int i=0;i<node.size();i++){
                if(ar[node.at(i).from]+node.at(i).edgeweight<ar[node.at(i).to]) ar[node.at(i).to] = ar[node.at(i).from]+node.at(i).edgeweight;
            }
        }
    }
    ftime = omp_get_wtime();
    etime = ftime-itime;
    cout<<"Ran for "<<etime<<"s"<<endl;
    return 1;
}

void SSSPNEW(int* ar,vector<int> sum,vector<edge> node,int to,int oldedge){
    int i=0;
    int proedge =0;
    for(int i=sum[to];i<sum[to+1];i++){
        if(oldedge+node.at(i).edgeweight==ar[node.at(i).to]) {
            proedge = ar[node.at(i).to];
            for(int j=0;j<node.size();j++){
                if( node.at(j).to == node.at(i).to && ar[node.at(j).from]+node.at(j).edgeweight<ar[node.at(i).to]){
                    // if(node.at(i).to==0) continue;
                    ar[node.at(i).to] = ar[node.at(j).from]+node.at(j).edgeweight;
                    SSSPNEW(ar,sum,node,node.at(i).to,proedge);
                }
            }
        }
    }
}

void DYNSSSPDEL(int* ar,vector<int> sum,vector<edge> node,Edge E){
    int oldedge = ar[E.to];
    for(int i=sum[E.from];i<sum[E.from+1];i++){
        if(node.at(i).to==E.to){
            node.erase(node.begin()+i-1);
            ar[E.to] = INT_MAX;
            for(int j = E.from+1;j<sum.size();j++){
                sum[j]--;
            }
            break;
        }
    }
    if(ar[E.from] + E.edgeweight != ar[E.to]) return;
    // for(auto it:sum) cout<<it<<endl;
    int newfrom = 0;
    for(int i=0;i<node.size();i++){
        if(node.at(i).to == E.to){
            if(ar[node.at(i).from]+node.at(i).edgeweight<ar[node.at(i).to]){
                // if(node.at(i).to==0) continue;
                ar[node.at(i).to] = ar[node.at(i).from]+node.at(i).edgeweight;
                newfrom = node.at(i).from;
            }
        }
    }    
    SSSPNEW(ar,sum,node,E.to,oldedge);
    return;
}


int main(int argc,char** argv){
    srand(0);
    omp_set_num_threads(70);
    vector<edge> node ;  //to store the crs input of the graph
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
    int ar[vertex];
    fill(ar,ar+vertex,INT_MAX);
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
    cout<<endl<<endl;
    srand((unsigned) time(NULL));
    int newedges = stoi(argv[4]);
    vector<Edge> addedge;
    // fstream inputfile1(argv[4],ios::in);
    for(int i=0;i<newedges;i++){
        Edge random = node.at(rand()%13289);
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
    cout<<"Time for one edge change "<<etime/newedges<<"s"<<endl;
    cout<<"The shortest distance from the starting node to all other nodes is\n"<<endl;
    for(auto it : ar) {
        if(it==INT_MAX){
            cout<<"max ";
            continue;
        }
        cout<<it<<" ";
        }
    return 0;
}