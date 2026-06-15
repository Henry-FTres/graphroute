#include <unordered_map>
#include <unordered_set>

namespace graph{

template<typename T>
class Graphroute{

private:
    struct node{
        T value;
        int grauEntrada = 0; //dentro do nó ou fora na classe como um unordered map???
        std::unordered_set<node*> links;
    };
    std::unordered_map<T, node> graph;

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
    
};
}