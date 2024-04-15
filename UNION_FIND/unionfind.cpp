#include <bits\stdc++.h>
#include <omp.h>

using namespace std;

struct Edge{
    int from;
    int to;
    float edgeweight;
};

bool Compare(Edge e1, Edge e2){
    return e1.from < e2.from;
}

class Union{
    private:
        int *size,*parent,n,vertex;
    public:
        Union(int n){
            parent = new int[n];
            size = new int[n];
            this->n = n;
            this->vertex = n;
            for(int i=0;i<n;i++){
                parent[i] = i;
                size[i] = 1;
            }
            // this->print();
            return;
        }

        int find(int node){
            if(parent[node]==node) return node;
            else {
                int term = this->find(parent[node]);
                parent[node] = term;
                return term;
            }
        }

        void unionbysize(int x,int y){
            int px = this->find(x);
            int py = this->find(y);
            int sx = size[x];
            int sy = size[y];
            if(px == py) return;
            if(sx < sy){
                parent[x] = y;
                size[x] += sy;
            }
            else{
                parent[y] = x;
                size[y] += sx;
            }
            this->n--;
            return;
        }

        void connected(vector<Edge>* node,int edges){
            for(int i=0;i<edges;i++){
                this->unionbysize(node->at(i).from,node->at(i).to);
            }
        }

        void print(){
            unordered_map<int,vector<int>> components;
            for(int i=0;i<this->vertex;i++){
                if(components.find(parent[i])!=components.end()){
                    components[parent[i]].push_back(i);
                }
                else{
                    components[parent[i]] = {i};
                }
            }
            int n=1;
            for(auto &pair:components){
                cout<<"Connected components "<<n++<<"\n";
                for(auto it:pair.second) cout<<it<<" ";
                cout<<"\n";
            }
        }
};

int main(int argc,char** argv){
    srand(0);
    omp_set_num_threads(70);
    vector<Edge> node ;  //to store the csr input of the graph
    vector<int> sum;
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
    Union concomp(vertex);
    double itime,ftime,etime;
    itime = omp_get_wtime();
    int i=0;
    // auto start = chrono::high_resolution_clock::now();
    // #pragma omp parallel for
    // for(i = 0; i < edge; i++) {
    //     #pragma omp critical
    //     {
    //         concomp.unionbysize(node.at(i).from, node.at(i).to);
    //     }
    // }
    #pragma omp parallel
    {
        #pragma omp for
        for(i = 0; i < edge; i++) {
            concomp.unionbysize(node.at(i).from, node.at(i).to);
        }
    }
        
    // auto end = chrono::high_resolution_clock::now();
    // double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    // time_taken *= 1e-9;
    // cout << "Time taken by program is : " << fixed 
    //      << time_taken << setprecision(9);
    // cout << " sec" << endl;
    ftime = omp_get_wtime();
    etime = ftime-itime;
    cout<<"Total time "<<etime<<"s\n";
    concomp.print();
    return 0;
}