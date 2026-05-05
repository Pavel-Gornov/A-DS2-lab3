#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <unordered_set>
#include <queue>
#include <functional>
#include <iostream>
#include <ostream>
#include "HashMap.h"

template<typename Vertex, typename Distance = double>
class Graph {
public:
    struct Edge {
        Edge(const Vertex& from, const Vertex& to, const Distance& d = Distance(1)): from(from), to(to), d(d) {}
        Vertex from = Vertex();
        Vertex to = Vertex();
        Distance d = Distance();
    };

    bool has_vertex(const Vertex& v) const {
        return data.contains_key(v);
    }
    bool add_vertex(const Vertex& v) {
        return data.insert(v, std::vector<Edge>());
    }
    bool remove_vertex(const Vertex& v) {
        return data.erase(v);
    }
    std::vector<Vertex> vertices() const {
        return data.keys();
    }

    void add_edge(const Vertex& from, const Vertex& to, const Distance& d) {
        if (!data.contains_key(from))
            data.insert(from, std::vector<Edge>());
        if (!data.contains_key(to))
            data.insert(to, std::vector<Edge>());
        if (!has_edge(from, to))
            data[from].push_back(Edge(from, to, d));
    }

    bool remove_edge(const Vertex& from, const Vertex& to) {
        std::vector<Edge>& edges = data[from];
        for (auto it = edges.begin(); it != edges.end(); it++) {
            if (it->to == to) {
                edges.erase(it);
                return true;
            }
        }
        return false;
    }

    bool remove_edge(const Edge& e) {
        std::vector<Edge>& edges = data[e.from];
        for (auto it = edges.begin(); it != edges.end(); it++) {
            if (*it == e) {
                edges.erase(it);
                return true;
            }
        }
        return false;
    }

    bool has_edge(const Vertex& from, const Vertex& to) const {
        const std::vector<Edge>& edges = data[from];
        for (auto it = edges.begin(); it != edges.end(); it++) {
            if (it->to == to) {
                return true;
            }
        }
        return false;
    }

    bool has_edge(const Edge& e) const {
        const std::vector<Edge>& edges = data[e.from];
        for (auto it = edges.begin(); it != edges.end(); it++) {
            if (*it == e) {
                return true;
            }
        }
        return false;
    }

    std::vector<Edge> edges(const Vertex& vertex) {
        return data[vertex];
    }

    size_t order() const {
        return data.size();
    }

    size_t degree(const Vertex& v) const { // TODO: Создать степени вхождения и выхождения.
        return data[v].size();
    }
    
    bool is_connected() const { //является ли граф сильно связным
        std::unordered_set<Vertex> visited;
        std::vector<Vertex> ord;
        std::function<void(const Vertex&)> ord_f = [&](const Vertex& v) { ord.push_back(v); };

        std::vector<Vertex> keys = data.keys();
        for (auto v = keys.begin(); v != keys.end(); v++) {
            if (!visited.contains(*v)) {
                std::vector<Vertex> temp;
                dfs_recursive_back_(*v, visited, temp, ord_f);
            }
        }

        Graph H = this->inverted_edges();
        size_t col = 1;
        HashMap<Vertex, size_t> cols;
        for (auto v = keys.begin(); v != keys.end(); v++) {
            cols[*v] = 0;
        }
        visited.clear();

        std::function<void(const Vertex&)> col_f = [&](const Vertex& v) { cols[v] = col;};

        for (auto riter = ord.rbegin(); riter != ord.rend(); ++riter) {
            if (!visited.contains(*riter)) {
                std::vector<Vertex> temp;
                H.dfs_recursive_(*riter, visited, temp, col_f);
                col++;
            }
        }
        return !cols.contains_value(2);
    } 

    std::vector<Edge> shortest_path(const Vertex& from, const Vertex& to) const {
        /* Алгоритм Дейкстры */
        std::unordered_set<Vertex> visited;
        HashMap<Vertex, Distance> d;
        HashMap<Vertex, std::vector<Edge>> paths;

        std::vector<Vertex> keys = data.keys();
        for (auto v = keys.begin(); v != keys.end(); v++) {
            d[*v] = Distance(INFINITY);
            paths[*v] = std::vector<Edge>();
        }
        d[from] = 0;

        std::priority_queue<Vertex> queue;
        queue.push(from);
        Vertex v = from;
        for (size_t it = 0; it < keys.size(); it++) {
            if (queue.empty())
                break;
            v = queue.top();
            queue.pop();

            visited.insert(v);

            const std::vector<Edge>& edges = data[v];
            for (size_t i = 0; i < edges.size(); i++) {
                if (d[v] + edges[i].d < d[edges[i].to]) {
                    if (!visited.contains(edges[i].to)) {
                        queue.push(edges[i].to);
                    }
                    d[edges[i].to] = d[v] + edges[i].d;
                    paths[edges[i].to] = paths[v];
                    paths[edges[i].to].push_back(edges[i]);
                }
            }
        }
        // Визуализация для отладки :)
        // for (auto v = keys.begin(); v != keys.end(); v++) {
        //     std::vector<Edge>& path = paths[*v];
        //     std::cout << *v << ": ";
        //     for (auto it = path.begin(); it != path.end(); it++) {
        //         std::cout << "{" << it->from << " -> " << it->to << ", " << "d: " << it->d << "} ";
        //     }
        //     std::cout << "\n";
        // }

        return paths[to];
    }

    HashMap<Vertex, Distance> dijkstra_lengths(const Vertex& from) {
        std::unordered_set<Vertex> visited;
        HashMap<Vertex, Distance> d;

        std::vector<Vertex> keys = data.keys();
        for (auto v = keys.begin(); v != keys.end(); v++) {
            d[*v] = Distance(INFINITY);
        }
        d[from] = 0;
        std::priority_queue<Vertex> queue;
        queue.push(from);
        Vertex v = from;
        for (size_t it = 0; it < keys.size(); it++) {
            if (queue.empty())
                break;
            v = queue.top();
            queue.pop();

            visited.insert(v);

            const std::vector<Edge>& edges = data[v];
            for (size_t i = 0; i < edges.size(); i++) {
                if (d[v] + edges[i].d < d[edges[i].to]) {
                    if (!visited.contains(edges[i].to)) {
                        queue.push(edges[i].to);
                    }
                    d[edges[i].to] = d[v] + edges[i].d;
                }
            }
        }
        return d;
    }
    
    std::vector<Vertex> walk(const Vertex& start_vertex, std::function<void(const Vertex&)> action) const {
        /* Обход в глубину */
        std::unordered_set<Vertex> visited;
        std::vector<Vertex> order;
        dfs_recursive_(start_vertex, visited, order, action);

        std::vector<Vertex> keys = data.keys();
        for (auto v = keys.begin(); v != keys.end(); v++) {
            dfs_recursive_(*v, visited, order, action);
        }

        return order;
    }

    void print() {
        std::vector<Vertex> keys = data.keys();
        for (auto v = keys.begin(); v != keys.end(); v++) {
            const std::vector<Edge>& edges = data[*v];
            if (edges.empty()) continue;
            std::cout << *v << ": [";
            for (size_t i = 0; i < edges.size(); i++) {
                const Edge& e = edges[i];
                std::cout << "{-> " << e.to << ", " << "d: " << e.d << "}";
                if (i != edges.size() - 1)
                    std::cout << ", ";
            }
            std::cout << "]\n";
        }
    }

    Graph inverted_edges() const {
        Graph inverse = Graph();

        std::vector<Vertex> keys = data.keys();
        for (auto v = keys.begin(); v != keys.end(); v++) {
            const std::vector<Edge>& edges = data[*v];
            for (size_t i = 0; i < edges.size(); i++) {
                const Edge& e = edges[i];
                inverse.add_edge(e.to, e.from, e.d);
            }
        }
        return inverse;
    }

private:
    HashMap<Vertex, std::vector<Edge>> data;

    void dfs_recursive_(const Vertex& vertex, std::unordered_set<Vertex>& visited, std::vector<Vertex>& order, std::function<void(const Vertex&)>& action) const {
        if (visited.contains(vertex)) return;
        visited.insert(vertex);
        order.push_back(vertex);
        action(vertex);
        const std::vector<Edge>& edges = data[vertex];
        for (size_t i = 0; i < edges.size(); i++) {
            if (!visited.contains(edges[i].to))
                dfs_recursive_(edges[i].to, visited, order, action);
        }
    }

    void dfs_recursive_back_(const Vertex& vertex, std::unordered_set<Vertex>& visited, std::vector<Vertex>& order, std::function<void(const Vertex&)>& action) const {
        if (visited.contains(vertex)) return;
        visited.insert(vertex);
        order.push_back(vertex);
        
        const std::vector<Edge>& edges = data[vertex];
        for (size_t i = 0; i < edges.size(); i++) {
            if (!visited.contains(edges[i].to))
                dfs_recursive_(edges[i].to, visited, order, action);
        }
        action(vertex);
    }
};


#endif /* GRAPH_H */