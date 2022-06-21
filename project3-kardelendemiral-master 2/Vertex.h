//
// Created by Kardelen Demiral on 19.12.2020.
//

#ifndef PROJECT3_VERTEX_H
#define PROJECT3_VERTEX_H

#include <vector>
#include <set>
#include <queue>
using namespace std;

class Vertex {
public:
    int id;
    int inDegree;
    int outDegree;
    queue<int> adjacencyList;
    Vertex(int id_);


};


#endif //PROJECT3_VERTEX_H