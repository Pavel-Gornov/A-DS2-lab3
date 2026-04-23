#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
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

    // size_t order() const; //порядок 
    // size_t degree(const Vertex& v) const; //степень вершины
    // bool is_connected() const; //является ли граф сильно связным

    // std::vector<Edge> shortest_path(const Vertex& from, const Vertex& to) const; 
    
    // std::vector<Vertex> walk(const Vertex& start_vertex, std::function<void(const Vertex&)> action) const; //обход
private:
    HashMap<Vertex, std::vector<Edge>> data;
};

#endif /* GRAPH_H */