#include <iostream>
#include <fstream>
#include "Vertex.h"
#include <queue>
#include <deque>
#include <stack>
#include <list>

int findNextVertex(list<int> &circuit, vector<Vertex> &graph, list<int>::iterator &it){
    for(;it!=circuit.end();it++){
        if(!graph[*it].adjacencyList.empty()){
            return *it;
        }
    }
    return -1;
}
void merge(list<int> &circuit,vector<int> &tour,list<int>::iterator &it){

    
    int pivot=tour[tour.size()-1];

    for(;it!=circuit.end();it++){
        if(*it==pivot){
            break;
        }
    }
    list<int>::iterator it2 = it;
    it2++;

    circuit.insert(it2,tour.begin(),tour.end());

}

bool isEulerian( vector<Vertex> &graph) {
    for(int i=0;i<graph.size();i++){
        if(graph[i].inDegree!=graph[i].outDegree){
            return false;
        }
    }
    return true;
}
int main(int argc, char* argv[]) {

    string infileName=argv[1];
    string outfileName=argv[2];

    ifstream infile;
    ofstream outfile;

    infile.open(infileName);
    outfile.open(outfileName);

    int v;
    int edges=0;
    infile >> v;

    vector<Vertex> graph(v, Vertex(0));
    
    bool eulerian=true;

    for(int i=0;i<v;i++){
        int id;
        int outDegree_;

        infile >> id >> outDegree_;
        graph[id].id==id;
        graph[id].outDegree=outDegree_;

        for(int j=0;j<outDegree_;j++){
            edges++;
            int outId;
            infile >> outId;
            graph[id].adjacencyList.push(outId);
            graph[outId].inDegree++;
            if(outId<=i && graph[outId].inDegree> graph[outId].outDegree){
                eulerian=false;
                break;
            }
            
        }
        if(!eulerian){
            break;
        }
    }

    
    int currentId;
    infile >> currentId;

    if(eulerian){
        eulerian=isEulerian(graph);
    }
    

    if(!eulerian){
        outfile << "no path";
    } else {
        list<int> eulerianCircuit;


        eulerianCircuit.push_back(currentId);
        list<int>::iterator lastIn=eulerianCircuit.begin();


        while(eulerianCircuit.size()<=edges){
            list<int> tour;

            while(!graph[currentId].adjacencyList.empty()){
                int nextId=graph[currentId].adjacencyList.front();
                graph[currentId].adjacencyList.pop();
                currentId=nextId;
                tour.push_back(currentId);
            }

            list<int>::iterator it=lastIn;
            it++;
            eulerianCircuit.splice(it,tour);

            for(;lastIn!=eulerianCircuit.end();lastIn++){
                if(!graph[*lastIn].adjacencyList.empty()){
                    currentId=*lastIn;
                    break;
                }
            }
        }

        for(auto x : eulerianCircuit){
            outfile << x <<" ";
        }



    }







    return 0;
}