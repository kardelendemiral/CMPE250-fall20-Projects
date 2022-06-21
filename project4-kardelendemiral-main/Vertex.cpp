#include "Vertex.h"

void Vertex::addEdge(int to,  int cost) {
    edges[to]=cost;
}

void Vertex::eraseEdge(int id) {
    edges.erase(id);
}

