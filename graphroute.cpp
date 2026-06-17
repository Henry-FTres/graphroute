#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <queue>
#include <vector>
 
namespace graph {
 
template<typename T>
class GraphRoute{
 
 
private:
 
    struct node{
        T value;
        std::unordered_set<node*> links;
    };
 
 
    std::unordered_map<T, node> graph;

    std::unordered_map<T, int> inDegrees;
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
 
 
        if(pfrom->links.count(pto)==0){ //poderia ter verificado com o próprio insert pois ele retorna um std::pair<iterator,bool>, esse bool diz se inseriu ou se já tinha. Ficaria: if (pfrom->links.insert(pto).second) e incrementa os outros 2 dentro do if
            pfrom->links.insert(pto);
            inDegrees[pto->value]++;
            total_edges++;
        }
    }
 
    void gerar_dot() {
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
    }
 
public:

    //adiciona dois vértices e uma aresta que liga do from para o to
    void insert_edge(const T& hop_from, const T& hop_to) { 
        insert_node(hop_from);
        insert_node(hop_to);
        insert_link(hop_from, hop_to);
    }
 
    int vertex_count() {
        return graph.size();
    }
 
   
 
    void showScreen() {
        gerar_dot();
        system("dot -Tx11 /tmp/graphviz.dot");
    }
 
    void showPng(string current_path) {
        std::string command = "dot -Tpng /tmp/graphviz.dot -o ";
        command += current_path;
        command += ".png";
        gerar_dot();
        system(command.c_str());
    }
 
    void showDoc(string current_path) {
        std::string command = "dot -Tpdf /tmp/graphviz.dot -o ";
        command += current_path;
        command += ".pdf";
        gerar_dot();
        system(command.c_str());
    }
 
    int edge_count() {
        return total_edges;
    }
 
    std::vector<node *> shortest_path(const T& start, const T& end)
    {
        std::vector<node *> path;
 
        auto pstart = find(start);
        if (!pstart)
            return path;
 
        auto pend = find(end);
        if (!pend)
            return path;
 
        std::queue<node*> queue;
        std::unordered_set<node*> queued;
        std::unordered_map<node*, node*> origin;
        queue.push(pstart);
        queued.insert(pstart);
        origin[pstart] = nullptr;
        bool found = false;
 
        while (!queue.empty()) {
            auto current = queue.front();
            queue.pop();
            if (current == pend){
                found = true;
                break;
            }
            for (auto adj : current->links) {
                if (queued.count(adj) == 0) {
                    queue.push(adj);
                    queued.insert(adj);
                    origin[adj] = current;
                }
            }            
        }
        if(found) {
            auto p = pend;
            while(p){
                path.push_back(p);
                p = origin[p];
            }
        }
        std::reverse(path.begin(), path.end());
 
        return path;
    }
 
};
}
 
 
