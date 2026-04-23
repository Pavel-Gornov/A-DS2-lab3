#include <iostream>
#include "Graph.h"

int main() {
    Graph<int> g = Graph<int>();
    g.add_edge(1, 2, 3.0);
    g.add_edge(1, 3, 2.0);

    std::cout << g.remove_edge(1, 2) << "\n";
    std::cout << g.has_edge(1, 2) << "\n";
    std::cout << "Hello World\n";
}