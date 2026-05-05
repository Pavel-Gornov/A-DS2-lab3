#include <iostream>
#include "Graph.h"


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
    // g.print();
}