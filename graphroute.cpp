#include <unordered_map>
#include <unordered_set>
#include <fstream>

namespace graph{

template<typename T>
class Graphroute{

private:

    struct node{
        T value;
        int inDegree = 0; //dentro do nó ou fora na classe como um unordered map???
        std::unordered_set<node*> links;
    };

    std::unordered_map<T, node> graph;
    int total_edges = 0;

    node* find(const T& val){
        auto it = graph.find(val);
        if (it == graph.end()) { //se chagar aqui quer dizer que não achou, por isso retorna nulo
            return nullptr;
        }
        return &it->second; //se encontrou retorna o nó
    }

    void insert_node(const T& val){
        if (graph.count(val) != 0)
            return;
        node aux;
        aux.value = val;
        graph[val] = aux;
    }

    void insert_link(const T& from, const T& to){
        auto pfrom = find(from);
        if (!pfrom)
            return;
        auto pto = find(to);
        if (!pto)
            return;

        if(pfrom->links.count(pto)==0){
            pfrom->links.insert(pto);
            pto->inDegree++;
            total_edges++;
        }
    }

public:

    
    
    //adiciona dois vértices e uma aresta que liga do from para o to
    void insert_edge(const T& hop_from, const T& hop_to) {
        if (graph.find(hop_from) || graph.find(hop_to))
        insert_node(hop_from);
        insert_node(hop_to);
        insert_link(hop_from, hop_to);
    }

    int vertex_count() {
        return graph.size();
    }

    void show()
    {
        std::ofstream dot("/tmp/graphviz.dot");
        dot << "digraph{\n";
        for (const auto& [key, node] : graph) {
            dot << "\t\"" << key << "\" -> {";
            for (const auto& link : node.links) {
                dot << "\"" << link->value << "\" ";
            }
            dot << "};\n";
        }
        dot << "}\n";
        dot.close();
        system("dot -Tx11 /tmp/graphviz.dot");
    }

    int edge_count() {
        return total_edges;
    }



};
}