#include <iostream>
#include "Graph.h"

template<typename Vertex, typename Distance = double> 
Vertex find_center(Graph<Vertex,Distance> g) {
    std::vector<Vertex> keys = g.vertices();

    HashMap<Vertex, Distance> excents;
    for (auto v = keys.begin(); v != keys.end(); v++) {
        HashMap<Vertex, Distance> d_v = g.dijkstra_lengths(*v);
        
        Distance max_dist = Distance(0);
        std::vector<Vertex> k = d_v.keys();
        for (auto it = k.begin(); it != k.end(); it++) {
            if (d_v[*it] > max_dist) max_dist = d_v[*it];
        }
        excents[*v] = max_dist;
    }

    Distance min_r = Distance(INFINITY);
    Vertex min_v;
    for (auto v = keys.begin(); v != keys.end(); v++) {
        if (excents[*v] < min_r) {
            min_r = excents[*v];
            min_v = *v;
        }
    }
    return min_v;
}


int main() {
    Graph<int> g = Graph<int>();
    g.add_edge(1, 2, 3.0);
    g.add_edge(1, 3, 2.0);
    g.add_edge(2, 1, 5.1);
    g.add_edge(2, 3, 3);
    g.add_edge(3, 9, 3);

    std::function<void(const int&)> order_c = [g](const int& v) { g.degree(v); };
    std::function<void(const int&)> no = [](const int& v) {};

    std::vector<int> res = g.walk(1, no);


    std::vector<Graph<int>::Edge> res2 = g.shortest_path(1, 9);
    for (auto it = res2.begin(); it != res2.end(); it++) {
        std::cout << "{" << it->from << " -> " << it->to << ", " << "d: " << it->d << "} ";
    }
    std::cout << "\n";
    g.print();
    Graph<int> g1 = g.inverted_edges();
    std::cout << "\n";
    g1.print();

    std::cout << "g is_connected: " << g.is_connected() << "\n";

    Graph<int> con;
    con.add_edge(1, 2, 1.0); con.add_edge(2, 3, 1.0); con.add_edge(3, 4, 1.0); con.add_edge(4, 1, 1.0);
    std::cout << "con is_connected: " << con.is_connected() << "\n";

    std::cout << "find_center(g): " << find_center(g) << "\n";
}