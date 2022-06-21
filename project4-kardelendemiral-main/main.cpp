#include <iostream>
#include <fstream>
#include "Vertex.h"
#include <vector>
#include <limits>
#include <stack>
#include <queue>
#include <deque>
#define max numeric_limits<int>::max()

using namespace std;


int detectCycle( int pre[],int n){
    for(int i=0;i<n;i++){
        if(pre[i]==-1){
            continue;
        }
        int size=0;
        size++;
        //visited[pre[i]]=true;
        int o=pre[i];
        while(size<n){
            o=pre[o];
            if(o==-1){
                break;
            }
            if(o==i){
                return o;
            }
            size++;
        }
    }
    return -1;

}


stack<int> trace(int pre[],int v , int n){
    vector<bool> inSt(n,false);
    stack<int> stck;
    while(!inSt[v]){
        stck.push(v);
        inSt[v]=true;
        v=pre[v];
    }
    stack<int> cycle; 
    cycle.push(v);
    while(stck.top()!=v){
        cycle.push(stck.top());
        stck.pop();
    }
    cycle.push(v);
    return cycle; //cycle 1->2,2->3,3->1 means stack (top) 1,3,2,1 (bottom)


}
// I used the algorithm in the link below for this function
// https://konaeakira.github.io/posts/using-the-shortest-path-faster-algorithm-to-find-negative-cycles.html
bool SFPA(vector<Vertex> & graph, vector<vector<int>> & costs, int n){
    int dis[2*n];
    int pre[2*n];
    bool inQ[2*n];

    queue<int> queue;
    for(int j=0;j<graph.size();j++){
        dis[j]=0;
        pre[j]=-1;
        queue.push(j);
        inQ[j]=true;

    }
    int iter=0;
    while(!queue.empty() ){
        int u=queue.front();
        queue.pop();
        inQ[u]=false;
        for(auto edge : graph[u].edges){
            int v=edge.first;
            if(dis[u] + edge.second < dis[v]){
                pre[v]=u;
                dis[v]=dis[u]+edge.second;
                iter++;
                if(iter==2*n){
                    iter=0;
                    int pivot = detectCycle(pre,2*n);

                    if(pivot != -1){
                        stack<int> cycle = trace(pre,pivot,2*n);


                        //HANDLE THE CYCLE
                        while(cycle.size() > 1){
                            int from=cycle.top();
                            cycle.pop();
                            int to=cycle.top();
                            graph[to].eraseEdge(from);

                            if(to<n){ 
                                int cost=(-1)*costs[to][from-n];
                                graph[from].addEdge(to,cost);
                            } else {
                                int cost=costs[from][to-n];
                                graph[from].addEdge(to,cost);
                            }

                        }

                        return true;
                    }

                }
                if(!inQ[v]){
                    queue.push(v);
                    inQ[v]=true;
                }

            }
        }
    }
    int pivot =detectCycle(pre,2*n);
    if(pivot != -1){
        stack<int> cycle = trace(pre,pivot,2*n);


        //HANDLE THE CYCLE
        while(cycle.size() > 1){
            int from=cycle.top();
            cycle.pop();
            int to=cycle.top();
            graph[to].eraseEdge(from);

            if(to<n){ 
                int cost=(-1)*costs[to][from-n];
                graph[from].addEdge(to,cost);
            } else {
                int cost=costs[from][to-n];
                graph[from].addEdge(to,cost);
            }

        }
        return true;

    }
    return false;
}
int main(int argc, char* argv[]) {

    string infileName=argv[1];
    string outfileName=argv[2];

    ifstream infile;
    ofstream outfile;

    infile.open(infileName);
    outfile.open(outfileName);

    int tests;
    infile >> tests;

    for(int test=0;test<tests;test++){
        int n;
        infile >> n;

        vector<vector<int>> costs;

        for(int i=0;i<n;i++){
            vector<int> v;
            for(int k=0;k<n;k++){
                int x;
                infile >> x;
                v.push_back((-1)*x);
            }
            costs.push_back(v);
        }

        vector<Vertex> graph(2*n,Vertex());

        for(int i=0;i<2*n;i++){
            graph[i].id=i;
        }

        for(int i=0;i<n;i++){
            graph[i+n].addEdge(i,(-1)*costs[i][i]);
            for(int j=n;j<graph.size();j++){
                if(j!=i+n){
                    graph[i].addEdge(j,costs[i][j-n]);
                }
            }
        }

        //SFPA
        bool neg=true;

        while(neg){
            neg=SFPA(graph,costs,n);
        }
        
        int totFlow=0;

        for(int i=n;i<graph.size();i++){
            for(auto v: graph[i].edges){
                int to = v.first;
                totFlow+=(-1)*costs[to][i-n];
            }
        }

        outfile << totFlow << endl;



    }



    return 0;
}