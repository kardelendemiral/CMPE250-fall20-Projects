#ifndef PROJECT4_VERTEX_H
#define PROJECT4_VERTEX_H

#include <vector>
#include <utility>
#include <unordered_map>
#include <set>

using namespace std;
class Vertex {
public:
    int id;
    unordered_map<int, int> edges;
    void addEdge(int to, int cost);
    void eraseEdge(int id);

};


#endif //PROJECT4_VERTEX_H