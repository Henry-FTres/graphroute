#include <unordered_map>
#include <unordered_set>

namespace graph{

template<typename T>
class Graphroute{

private:

    std::unordered_map<T, node> graph;
    int total_edges = 0;
    
    struct node{
        T value;
        int inDegree = 0; //dentro do nó ou fora na classe como um unordered map???
        std::unordered_set<node*> links;
    };


    node* find(const T& val){
        auto it = graph.find(val);
        if (it == graph.end()) { //se chagar aqui quer dizer que não achou, por isso retorna nulo
            return nullptr;
        }
        return &it->second; //se encontrou retorna o nó
    }

public:

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
            pto -> inDegree++;
        }
    }
    
    insert_edge(const T& hop_from, const T& hop_to) {
        insert_node(hop_from);
        insert_node(hop_to);
        insert_link(hop_from, hop_to);
        total_edges++;
    }
    
};
}